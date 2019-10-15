set(H264BITSTREAM_LIB_PATH "${THIRD_PART_LIB_DIR}/libh264bitstream.a")

add_library(NaluParser STATIC
    NaluParser.cpp
    NaluParser.h
)

message(${THIRD_PART_INCLUDE_DIR})

target_include_directories(NaluParser PUBLIC
    ${THIRD_PART_INCLUDE_DIR}
)

target_link_libraries(NaluParser
    ${H264BITSTREAM_LIB_PATH}
)
