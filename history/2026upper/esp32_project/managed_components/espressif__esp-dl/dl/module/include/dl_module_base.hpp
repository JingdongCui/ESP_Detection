#pragma once
#include "dl_base.hpp"
#include "dl_define.hpp"
#include "dl_model_context.hpp"
#include "dl_tensor_base.hpp"
#include "dl_tool.hpp"
#include "dl_tool_cache.hpp"
#include "fbs_model.hpp"
#include "freertos/idf_additions.h"
#include <functional>
#include <iostream>
#include <stdint.h>

namespace dl {
// Define the enum type for module in-place operation mode
typedef enum {
    MODULE_NON_INPLACE = 0, ///< Non inplace operation. the output will store to a separate memory
    MODULE_INPLACE_UNCHANGED_BUFFER =
        1,                            ///< Inplace operation which don't change the buffer data, like Reshape, Squeeze
    MODULE_INPLACE_CHANGED_BUFFER = 2 ///< Inplace operation which will change the buffer data, like Add, Sub
} module_inplace_t;

namespace module {
/**
 * @brief Base class for module.
 */
class Module {
public:
    char *name;                       ///< Name of module
    module_inplace_t inplace;         ///< Inplace type
    quant_type_t quant_type;          ///< Quantization type
    std::vector<int> m_inputs_index;  ///< Tensor index of model's tensors that used for inputs
    std::vector<int> m_outputs_index; ///< Tensor index of model's tensors that used for outputs

    /**
     * @brief Construct a new Module object.
     *
     * @param name Name of module.
     * @param inplace   Inplace operation mode
     * @param quant_type Quantization type
     */
    Module(const char *name = NULL,
           module_inplace_t inplace = MODULE_NON_INPLACE,
           quant_type_t quant_type = QUANT_TYPE_NONE);

    /**
     * @brief Destroy the Module object. Return resource.
     */
    virtual ~Module();

#if CONFIG_SPIRAM
    void *operator new(size_t size) { return tool::malloc_aligned(size, MALLOC_CAP_SPIRAM); }

    void operator delete(void *ptr) { heap_caps_free(ptr); }
#endif

    /**
     * @brief Get the tensor index of this module's outputs
     *
     * @return Tensor index of model's tensors
     */
    virtual std::vector<int> get_outputs_index() { return m_outputs_index; }

    /**
     * @brief Calculate output shape by input shape
     *
     * @param input_shapes   Input shapes
     *
     * @return outputs shapes
     */
    virtual std::vector<std::vector<int>> get_output_shape(std::vector<std::vector<int>> &input_shapes) = 0;

    /**
     * @brief Build the module, high-level interface for Module layer
     *
     * @param context   Model context including  all inputs and outputs and other runtime information
     * @param mode    Runtime mode, default is RUNTIME_MODE_AUTO
     */
    virtual void forward(ModelContext *context, runtime_mode_t mode = RUNTIME_MODE_AUTO) = 0;

    /**
     * @brief Run the module, Low-level interface for base layer and multi-core processing
     *
     * @param args      ArgsType, arithArgsType, resizeArgsType and so on
     */
    virtual void forward_args(void *args) {};

    /**
     * @brief create module instance by node serialization information
     *
     * @param fbs_model  Flatbuffer's model
     * @param node_name  The node name in model's graph
     *
     * @return The pointer of module instance
     */
    static Module *deserialize(fbs::FbsModel *fbs_model, std::string node_name) { return nullptr; }

    /**
     * @brief print module information
     */
    virtual void print() {}

    /**
     * @brief set preload RAM pointer
     *
     * @param addr Internal RAM address, should be aligned to 16 bytes
     * @param size The size of RAM address
     */
    virtual void set_preload_addr(void *addr, size_t size) {}

    /**
     * @brief Perform a preload operation
     *
     * @warning Not implemented
     */
    virtual void preload() {}

    /**
     * @brief reset all state of module, include inputs， outputs and preload cache setting
     */
    virtual void reset() {}

    /**
     * @brief Run the module with single input and single output
     *
     * @param input   Input tensor
     * @param output  Output tensor
     * @param mode    Runtime mode
     */
    virtual void run(TensorBase *input, TensorBase *output, runtime_mode_t mode = RUNTIME_MODE_SINGLE_CORE);

    /**
     * @brief Run the module by inputs and outputs
     *
     * @param inputs   Input tensors
     * @param outputs  Output tensors
     * @param mode    Runtime mode
     */
    virtual void run(std::vector<dl::TensorBase *> inputs,
                     std::vector<dl::TensorBase *> outputs,
                     runtime_mode_t mode = RUNTIME_MODE_SINGLE_CORE);
};

/**
 * @brief The data struct of module task. Pack all necessary information as the input for module task.
 */
typedef struct {
    Module *op;                   ///< Module instance pointer
    void *args;                   ///< ArgsType, arithArgsType, resizeArgsType and so on
    SemaphoreHandle_t &semaphore; ///< recommend xSemaphoreCreateCounting
} module_task_data_t;

/**
 * @brief The function of module task.
 * @param args The data of module task.
 */
static void module_forward_task(void *args)
{
    module_task_data_t *task = (module_task_data_t *)args;
    task->op->forward_args(task->args);
    xSemaphoreGive(task->semaphore);
    vTaskSuspend(NULL);
}

typedef struct {
    Module *op;
    void *args;
    SemaphoreHandle_t done;
} module_worker_job_t;

static module_worker_job_t s_module_worker_jobs[2];
static TaskHandle_t s_module_worker_handles[2] = {NULL, NULL};
static StaticSemaphore_t s_module_done_sem_buf;
static SemaphoreHandle_t s_module_done_sem = NULL;
static int s_module_worker_core_ids[2] = {0, 1};

static void module_forward_worker(void *args)
{
    int core_id = *(int *)args;
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        module_worker_job_t job = s_module_worker_jobs[core_id];
        if (job.op) {
            job.op->forward_args(job.args);
        }
        if (job.done) {
            xSemaphoreGive(job.done);
        }
    }
}

static bool module_forward_workers_ensure(UBaseType_t priority)
{
    if (!s_module_done_sem) {
        s_module_done_sem = xSemaphoreCreateCountingStatic(2, 0, &s_module_done_sem_buf);
        if (!s_module_done_sem) {
            return false;
        }
    }

    for (int core = 0; core < 2; core++) {
        if (s_module_worker_handles[core]) {
            continue;
        }
        char name[8];
        snprintf(name, sizeof(name), "dlw%d", core);
        if (xTaskCreatePinnedToCoreWithCaps(module_forward_worker,
                                            name,
                                            4096,
                                            &s_module_worker_core_ids[core],
                                            priority,
                                            &s_module_worker_handles[core],
                                            core,
                                            MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT) != pdPASS) {
            return false;
        }
    }
    return true;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
/**
 * @brief Run the module with dual core and use semaphores to keep tasks in sync
 *
 * @param op            Module instance
 * @param args1         Task1 args: ArgsType, arithArgsType, resizeArgsType and so on
 * @param args2         Task2 args: ArgsType, arithArgsType, resizeArgsType and so on
 */
static void module_forward_dual_core(Module *op, void *args1, void *args2)
{
    BaseType_t current_core_id = xPortGetCoreID();
    UBaseType_t current_priority = uxTaskPriorityGet(xTaskGetCurrentTaskHandle());
    if (current_core_id < 0 || current_core_id > 1 || !module_forward_workers_ensure(current_priority)) {
        op->forward_args(args1);
        op->forward_args(args2);
        return;
    }

    while (xSemaphoreTake(s_module_done_sem, 0) == pdTRUE) {
    }

    int other_core_id = (current_core_id + 1) % 2;
    s_module_worker_jobs[other_core_id] = {
        .op = op,
        .args = args1,
        .done = s_module_done_sem,
    };
    s_module_worker_jobs[current_core_id] = {
        .op = op,
        .args = args2,
        .done = s_module_done_sem,
    };

    xTaskNotifyGive(s_module_worker_handles[other_core_id]);
    xTaskNotifyGive(s_module_worker_handles[current_core_id]);
    xSemaphoreTake(s_module_done_sem, portMAX_DELAY);
    xSemaphoreTake(s_module_done_sem, portMAX_DELAY);
}
#pragma GCC diagnostic pop

} // namespace module
} // namespace dl
