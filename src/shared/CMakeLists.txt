# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.10)

project(shared)

if(OB_USE_SYSTEM_LIBS)
    message(STATUS "shared: Using system libraries")
else()
    message(STATUS "shared: Building third-party libraries")
    
    if(OB_BUILD_3RDPARTY_SPDLOG AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty/spdlog/CMakeLists.txt)
        add_subdirectory(../../../3rdparty/spdlog)
    endif()
    
    if(OB_BUILD_3RDPARTY_TINYXML2 AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty/tinyxml2/CMakeLists.txt)
        add_subdirectory(../../../3rdparty/tinyxml2)
    endif()
endif()

# 创建 shared 库（即使没有源文件）
add_library(shared INTERFACE)

# 添加系统库依赖
if(OB_USE_SYSTEM_LIBS)
    if(OB_SPDLOG_LIB)
        target_link_libraries(shared INTERFACE ${OB_SPDLOG_LIB})
    endif()
    if(OB_TINYXML2_LIB)
        target_link_libraries(shared INTERFACE ${OB_TINYXML2_LIB})
    endif()
    if(OB_FMT_LIB)
        target_link_libraries(shared INTERFACE ${OB_FMT_LIB})
    endif()
endif()

target_include_directories(shared INTERFACE ${OB_PUBLIC_HEADERS_DIR} ${CMAKE_CURRENT_LIST_DIR})

# 创建别名
add_library(ob::shared ALIAS shared)

message(STATUS "shared: Created interface library with system dependencies")