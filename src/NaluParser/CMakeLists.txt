#set(H264BITSTREAM_LIB_PATH "${THIRD_PART_LIB_DIR}/libh264bitstream.a")

add_library(NaluParser STATIC
    Enums.cpp
    Enums.h
    NaluParser.cpp
    NaluParser.h
    NalRecords/NalRecord.cpp
    NalRecords/NalRecord.h
    NalRecords/NalSPSRecord.cpp
    NalRecords/NalSPSRecord.h
)

#message(${THIRD_PART_INCLUDE_DIR})

target_link_libraries(NaluParser
    Utilities
)
