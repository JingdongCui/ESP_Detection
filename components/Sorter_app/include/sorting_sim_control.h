#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*sorting_sim_send_fn_t)(void *ctx, const char *line);

void sorting_sim_control_reset(void);
void sorting_sim_control_handle_line(const char *line, size_t len, sorting_sim_send_fn_t send_fn, void *send_ctx);
void sorting_sim_control_tick(sorting_sim_send_fn_t send_fn, void *send_ctx);
void sorting_sim_debug_start(void);

#ifdef __cplusplus
}
#endif
