cmake_minimum_required (VERSION 3.10.0)
project (VideoFileTools)

set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
if (NOT BUILD_TYPE)
    set(BUILD_TYPE "Default")
endif()


set(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/../output/${BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT}/lib")
set (CMAKE_CXX_FLAGS "--std=c++14 ${CMAKE_CXX_FLAGS}")

set(THIRD_PART_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdPart")
set(THIRD_PART_INCLUDE_DIR "${THIRD_PART_DIR}/include")

#include_directories(${THIRD_PART_INCLUDE_DIR})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(THIRD_PART_LIB_DIR "${THIRD_PART_DIR}/lib/linux")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(THIRD_PART_LIB_DIR "${THIRD_PART_DIR}/lib/macos")
else()
    message(SEND_ERROR "Unsupported OS")
endif()

add_subdirectory(NaluParser)

add_subdirectory(Utilities)

add_subdirectory(FlvEditor)

