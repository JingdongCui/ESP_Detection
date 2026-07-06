# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(sample_project)

# 当前不打包模型：vision 处于假框阶段，固件不挂载 storage 分区、不加载 .espdl。
# 需要模型时取消下行注释，并确保 model/ 内文件名 ≤ 32 字符(SPIFFS_OBJ_NAME_LEN)。
spiffs_create_partition_image(storage model FLASH_IN_PROJECT)
