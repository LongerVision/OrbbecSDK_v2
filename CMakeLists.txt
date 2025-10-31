# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.10)

project(OrbbecSDK VERSION 2.5.5 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# check if current project is a sub project of other project
if(OB_BUILD_MAIN_PROJECT)
    set(OB_IS_MAIN_PROJECT TRUE)
elseif(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(OB_IS_MAIN_PROJECT TRUE)
else()
    set(OB_IS_MAIN_PROJECT FALSE)
endif()

# ============ 统一的第三方库控制选项 ============
option(OB_USE_SYSTEM_LIBS "Use system libraries for all dependencies" ON)
option(OB_EMBED_RESOURCES "Embed resources into binaries using cmrc" OFF)

# 各个库的详细控制（当 OB_USE_SYSTEM_LIBS=OFF 时使用）
option(OB_BUILD_3RDPARTY_LIBJPEG "Build libjpeg from source" OFF)
option(OB_BUILD_3RDPARTY_LIBUSB "Build libusb from source" OFF)
option(OB_BUILD_3RDPARTY_JSONCPP "Build jsoncpp from source" OFF)
option(OB_BUILD_3RDPARTY_TINYXML2 "Build tinyxml2 from source" OFF)
option(OB_BUILD_3RDPARTY_LIBYUV "Build libyuv from source" OFF)
option(OB_BUILD_3RDPARTY_SPDLOG "Build spdlog from source" OFF)
option(OB_BUILD_3RDPARTY_LIBUVC "Build libuvc from source" OFF)
option(OB_BUILD_3RDPARTY_LIBFMT "Build libfmt from source" OFF)
option(OB_BUILD_3RDPARTY_LIVE555 "Build live555 from source" OFF)
option(OB_BUILD_3RDPARTY_DYLIB "Build dylib from source" OFF)
option(OB_BUILD_3RDPARTY_ROSBAG "Build rosbag from source" OFF)

# load project options
include(cmake/options.cmake)

# load macros
include(cmake/macro.cmake)

# set compiler flags
include(cmake/compiler_flags.cmake)

# configure platform-specific settings
if(ANDROID_NDK_TOOLCHAIN_INCLUDED)
    include(cmake/android_config.cmake)
elseif(WIN32)
    include(cmake/windows_config.cmake)
elseif(APPLE)
    include(cmake/macos_config.cmake)
elseif(UNIX)
    include(cmake/linux_config.cmake)
endif()

# configure global settings
include(cmake/global_config.cmake)

# configure global directories variables
set(OB_PUBLIC_HEADERS_DIR ${CMAKE_CURRENT_LIST_DIR}/include/)
set(OB_PROJECT_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

# 使用 find_package 统一查找所有系统库
if(OB_USE_SYSTEM_LIBS)
    message(STATUS "Using system libraries for all dependencies")
    
    # 查找 libjpeg
    find_package(JPEG QUIET)
    if(JPEG_FOUND)
        message(STATUS "Found system libjpeg: ${JPEG_VERSION}")
        set(OB_JPEG_LIB JPEG::JPEG)
    else()
        message(FATAL_ERROR "System libjpeg not found. Please install libjpeg-dev")
    endif()
    
    # 查找 libusb
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(LIBUSB libusb-1.0 QUIET)
        if(LIBUSB_FOUND)
            message(STATUS "Found system libusb via pkg-config")
            set(OB_LIBUSB_LIB ${LIBUSB_LIBRARIES})
        else()
            find_package(libusb-1.0 QUIET)
            if(libusb-1.0_FOUND)
                message(STATUS "Found system libusb")
                set(OB_LIBUSB_LIB libusb-1.0::libusb-1.0)
            else()
                message(FATAL_ERROR "System libusb not found. Please install libusb-1.0-0-dev")
            endif()
        endif()
    endif()
    
    # 查找 jsoncpp
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(JSONCPP jsoncpp QUIET)
        if(JSONCPP_FOUND)
            message(STATUS "Found system jsoncpp via pkg-config")
            set(OB_JSONCPP_LIB ${JSONCPP_LIBRARIES})
        else()
            find_package(jsoncpp QUIET)
            if(jsoncpp_FOUND)
                message(STATUS "Found system jsoncpp")
                set(OB_JSONCPP_LIB jsoncpp_lib)
            else()
                message(FATAL_ERROR "System jsoncpp not found. Please install libjsoncpp-dev")
            endif()
        endif()
    endif()
    
    # 查找 tinyxml2
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(TINYXML2 tinyxml2 QUIET)
        if(TINYXML2_FOUND)
            message(STATUS "Found system tinyxml2 via pkg-config")
            set(OB_TINYXML2_LIB ${TINYXML2_LIBRARIES})
        else()
            find_package(tinyxml2 QUIET)
            if(tinyxml2_FOUND)
                message(STATUS "Found system tinyxml2")
                set(OB_TINYXML2_LIB tinyxml2::tinyxml2)
            else()
                message(FATAL_ERROR "System tinyxml2 not found. Please install libtinyxml2-dev")
            endif()
        endif()
    endif()
    
    # 查找 libyuv
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(LIBYUV libyuv QUIET)
        if(LIBYUV_FOUND)
            message(STATUS "Found system libyuv via pkg-config")
            set(OB_LIBYUV_LIB ${LIBYUV_LIBRARIES})
        else()
            # 对于 libyuv，如果没找到就警告而不是报错
            message(WARNING "System libyuv not found, continuing without it")
            set(OB_LIBYUV_LIB "")
        endif()
    endif()
    
    # 查找 spdlog
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(SPDLOG spdlog QUIET)
        if(SPDLOG_FOUND)
            message(STATUS "Found system spdlog via pkg-config")
            set(OB_SPDLOG_LIB ${SPDLOG_LIBRARIES})
        else()
            # 对于 spdlog，如果没找到就警告而不是报错
            message(WARNING "System spdlog not found, continuing without it")
            set(OB_SPDLOG_LIB "")
        endif()
    endif()

    # 查找 fmt 库
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(FMT fmt QUIET)
        if(FMT_FOUND)
            message(STATUS "Found system fmt via pkg-config")
            set(OB_FMT_LIB ${FMT_LIBRARIES})
            set(OB_FMT_INCLUDE_DIRS ${FMT_INCLUDE_DIRS})
        else()
            find_package(fmt QUIET)
            if(fmt_FOUND)
                message(STATUS "Found system fmt")
                set(OB_FMT_LIB fmt::fmt)
            else()
                message(WARNING "System fmt not found, spdlog may not work properly")
                set(OB_FMT_LIB "")
            endif()
        endif()
    endif()

     # 查找 live555
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(LIVE555 live555 QUIET)
        if(LIVE555_FOUND)
            message(STATUS "Found system live555 via pkg-config")
            set(OB_LIVE555_LIB ${LIVE555_LIBRARIES})
        else()
            # 尝试其他方式查找
            find_library(LIVE555_LIBRARY NAMES live555 liveMedia groupsock BasicUsageEnvironment UsageEnvironment)
            if(LIVE555_LIBRARY)
                message(STATUS "Found system live555 manually: ${LIVE555_LIBRARY}")
                set(OB_LIVE555_LIB ${LIVE555_LIBRARY})
            else()
                message(WARNING "System live555 not found, RTSP features will be disabled")
                set(OB_LIVE555_LIB "")
            endif()
        endif()
    endif()
    
    # 查找 OpenCV (用于示例)
    if(OB_BUILD_EXAMPLES)
        find_package(OpenCV QUIET)
        if(OpenCV_FOUND)
            message(STATUS "Found system OpenCV: ${OpenCV_VERSION}")
            set(OB_OPENCV_LIB ${OpenCV_LIBS})
        else()
            message(WARNING "System OpenCV not found, examples may not build")
        endif()
    endif()
    
else()
    message(STATUS "Building all third-party libraries from source")
endif()

# 设置回退，如果没有找到任何库
foreach(lib JPEG LIBUSB JSONCPP TINYXML2 LIBYUV SPDLOG OPENCV)
    if(NOT DEFINED OB_${lib}_LIB)
        set(OB_${lib}_LIB "")
    endif()
endforeach()

# 在编译器标志中添加
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # 禁用零长度数组的 pedantic 警告
    add_compile_options(-Wno-pedantic)
    # 或者更精确地禁用特定警告
    add_compile_options(-Wno-zero-length-array)
endif()

add_subdirectory(extensions)
add_subdirectory(src)

if(OB_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(OB_INSTALL_EXAMPLES_SOURCE AND OB_IS_MAIN_PROJECT)
    install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/examples/ DESTINATION examples/src PATTERN "*publish_files*" EXCLUDE )
    if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/examples/publish_files)
        install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/examples/publish_files/CMakeLists.txt DESTINATION examples)
        if(UNIX)
            if(APPLE)
                install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/examples/publish_files/build_examples_macos.sh DESTINATION ./ RENAME build_examples.sh)
            else()
                install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/examples/publish_files/build_examples.sh DESTINATION ./)
            endif()
        endif()
    endif()
endif()

if(OB_BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(OB_BUILD_DOCS)
    add_subdirectory(docs)
endif()

if(OB_BUILD_TOOLS)
    add_subdirectory(tools)
endif()

add_subdirectory(scripts)

ob_print_summary()