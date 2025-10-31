# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.10)

# add the filter library with the version as the project
add_library(filter STATIC)
set_target_properties(filter PROPERTIES VERSION ${PROJECT_VERSION})

# add the source files
file(GLOB_RECURSE SOURCE_FILES "*.cpp" EXCLUDE unittest)
file(GLOB_RECURSE HEADERS_FILES "*.hpp" EXCLUDE unittest)
target_sources(filter PRIVATE ${SOURCE_FILES} ${HEADERS_FILES})

# dependencies
target_link_libraries(filter PUBLIC ob::shared ob::core ob::device)
target_include_directories(filter PUBLIC ${OB_PUBLIC_HEADERS_DIR} ${CMAKE_CURRENT_LIST_DIR})

# 只在需要时添加第三方库
if(NOT OB_USE_SYSTEM_LIBS)
    if(OB_BUILD_3RDPARTY_DYLIB AND EXISTS ${OB_3RDPARTY_DIR}/dylib/CMakeLists.txt)
        add_subdirectory(${OB_3RDPARTY_DIR}/dylib dylib)
        target_link_libraries(filter PUBLIC dylib::dylib)
    endif()
    
    if(OB_BUILD_3RDPARTY_LIBJPEG AND EXISTS ${OB_3RDPARTY_DIR}/libjpeg/CMakeLists.txt)
        add_subdirectory(${OB_3RDPARTY_DIR}/libjpeg libjpeg)
        target_link_libraries(filter PUBLIC libjpeg::libjpeg)
    endif()
    
    if(OB_BUILD_3RDPARTY_LIBYUV AND EXISTS ${OB_3RDPARTY_DIR}/libyuv/CMakeLists.txt)
        add_subdirectory(${OB_3RDPARTY_DIR}/libyuv libyuv)
        target_link_libraries(filter PUBLIC libyuv::libyuv)
    endif()
else()
    # 使用系统库
    if(OB_JPEG_LIB)
        target_link_libraries(filter PUBLIC ${OB_JPEG_LIB})
    endif()
    if(OB_LIBYUV_LIB)
        target_link_libraries(filter PUBLIC ${OB_LIBYUV_LIB})
    endif()
endif()

add_library(ob::filter ALIAS filter)
ob_source_group(ob::filter)

set_target_properties(filter PROPERTIES FOLDER "modules")

if(OB_INSTALL_FILTER_DEV_HEADERS)
    install(FILES ${CMAKE_CURRENT_LIST_DIR}/privatefilters/PrivFilterTypes.h DESTINATION include/libobsensor/h/)
endif()

message(STATUS "filter: Built with system libraries")