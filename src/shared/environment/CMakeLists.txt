# src/shared/environment/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(environment)

# 自动查找所有源文件
file(GLOB SOURCES "*.cpp" "*.c")
file(GLOB HEADERS "*.hpp" "*.h")

if(SOURCES)
    message(STATUS "environment: Building with sources: ${SOURCES}")
    add_library(environment ${SOURCES})
    
    if(HEADERS)
        target_include_directories(environment PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    endif()
    
    # 添加必要的链接库
    if(OB_USE_SYSTEM_LIBS AND OB_TINYXML2_LIB)
        target_link_libraries(environment ${OB_TINYXML2_LIB})
    endif()
else()
    # 如果没有源文件，创建空的目标
    message(STATUS "environment: No source files found, creating interface library")
    add_library(environment INTERFACE)
endif()