# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.10)

set(OB_TARGET_MEDIA media)

# add the media library with the version as the project
add_library(${OB_TARGET_MEDIA} STATIC)
set_target_properties(${OB_TARGET_MEDIA} PROPERTIES VERSION ${PROJECT_VERSION})

# 明确指定要编译的文件（排除 ros 目录）
set(SOURCE_FILES
    playback/PlaybackDepthWorkModeManager.cpp
    playback/PlaybackDevice.cpp
    playback/PlaybackDeviceParamManager.cpp
    playback/PlaybackDevicePort.cpp
    playback/PlaybackDeviceSyncConfigurator.cpp
    playback/PlaybackFilterCreationStrategy.cpp
    playback/PlaybackFrameInterleaveManager.cpp
    playback/PlaybackPresetManager.cpp
    playback/PlaybackPropertyAccessor.cpp
    record/RecordDevice.cpp
)

set(HEADERS_FILES
    playback/PlaybackDepthWorkModeManager.hpp
    playback/PlaybackDevice.hpp
    playback/PlaybackDeviceParamManager.hpp
    playback/PlaybackDevicePort.hpp
    playback/PlaybackDeviceSyncConfigurator.hpp
    playback/PlaybackFilterCreationStrategy.hpp
    playback/PlaybackFrameInterleaveManager.hpp
    playback/PlaybackPresetManager.hpp
    playback/PlaybackPropertyAccessor.hpp
    playback/StateMachineBase.hpp
    record/RecordDevice.hpp
)

target_sources(${OB_TARGET_MEDIA} PRIVATE ${SOURCE_FILES} ${HEADERS_FILES})

# dependencies
target_link_libraries(${OB_TARGET_MEDIA} PUBLIC ob::shared ob::core ob::device ob::platform)
target_include_directories(${OB_TARGET_MEDIA} PUBLIC ${OB_PUBLIC_HEADERS_DIR} ${CMAKE_CURRENT_LIST_DIR})

# dependencies:
if(NOT OB_USE_SYSTEM_LIBS)
    if(OB_BUILD_3RDPARTY_ROSBAG AND EXISTS ${OB_3RDPARTY_DIR}/rosbag/CMakeLists.txt)
        add_subdirectory(${OB_3RDPARTY_DIR}/rosbag rosbag)
        target_link_libraries(${OB_TARGET_MEDIA} PUBLIC rosbag::rosbag)
    endif()
endif()

# 跳过不存在的子目录
message(STATUS "media: Skipping playback, record, ros - no CMakeLists.txt found")

add_library(ob::media ALIAS ${OB_TARGET_MEDIA})
ob_source_group(ob::media)
set_target_properties(${OB_TARGET_MEDIA} PROPERTIES FOLDER "modules")