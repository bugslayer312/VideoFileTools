#include "Encoder.h"

#include <netinet/in.h>

#include <cstring>

// === ArchiveDownload::Omny::FlvEncoder ===

const uint32_t ArchiveDownload::Omny::FlvEncoder::frame_header_offset = 11;
const uint32_t ArchiveDownload::Omny::FlvEncoder::video_data_offset = 5;
const uint32_t ArchiveDownload::Omny::FlvEncoder::avc_decoder_config_offset = 6;

void ArchiveDownload::Omny::Reader::fix_frame_data(char* data, size_t data_count) {
    char* prevNaluStart = 0;
    uint32_t prevNaluSize = 0;
    char* searchPtr = data;
    while (searchPtr <= data + data_count - 4) {
        char* found = static_cast<char*>(memchr(searchPtr, 0, data_count - 4 - (searchPtr - data)));
        if (!found) {
            break;
        }
        searchPtr = found;
        if (ntohl(*reinterpret_cast<uint32_t*>(found)) == 1) {
            if (prevNaluStart) {
                prevNaluSize = static_cast<uint32_t>(found - prevNaluStart - 4);
                *reinterpret_cast<uint32_t*>(prevNaluStart) = htonl(prevNaluSize);
            }
            prevNaluStart = found;
            searchPtr += 4;
        } else {
            ++searchPtr;
        }
    }
    if (prevNaluStart) {
        prevNaluSize = static_cast<uint32_t>(data + data_count - prevNaluStart - 4);
        *reinterpret_cast<uint32_t*>(prevNaluStart) = htonl(prevNaluSize);
    }
}

ArchiveDownload::Omny::FlvEncoder::FlvEncoder() : prev_frame_size(0){
}

void ArchiveDownload::Omny::FlvEncoder::load_sps_pps_from_frame(FrameShp frame) {
    uint32_t pos = 0;
    while (pos + 4 < frame->header.size && (sps.empty() || pps.empty())) {
        uint32_t length = ntohl(*reinterpret_cast<uint32_t*>(frame->buffer.data + pos));
        pos += 4;
        if (pos + length > frame->header.size) {
            break;
        }
        uint8_t nalu_type = frame->buffer.data[pos] & 0x1f; // b00011111
        if (nalu_type == 0x07 && sps.empty()) {
            sps.resize(length);
            memcpy(&sps[0], frame->buffer.data + pos, length);
        } else if (nalu_type == 0x08 && pps.empty()) {
            pps.resize(length);
            memcpy(&pps[0], frame->buffer.data + pos, length);
        }
        pos += length;
    }
}

void ArchiveDownload::Omny::FlvEncoder::create_default_sps() {
    static const uint8_t default_sps[16] = {0x67, 0x4d, 0x00, 0x2a, 0x96, 0x35, 0x40, 0xf0,
                                            0x04, 0x4f, 0xcb, 0x37, 0x01, 0x01, 0x01, 0x02};
    sps.resize(16);
    memcpy(&sps[0], default_sps, 16);
}

void ArchiveDownload::Omny::FlvEncoder::create_default_pps() {
    static const uint8_t default_pps[4] = {0x68, 0xee, 0x3c, 0x80};
    pps.resize(4);
    memcpy(&pps[0], default_pps, 4);
}

void ArchiveDownload::Omny::FlvEncoder::reset_buffer(size_t size) {
    buffer.reset(new std::vector<uint8_t>());
    buffer->reserve(size);
}

void ArchiveDownload::Omny::FlvEncoder::write_uint32(uint32_t num) {
    buffer->resize(buffer->size() + 4);
    uint32_t* dest = reinterpret_cast<uint32_t*>(&(*buffer)[buffer->size() - 4]);
    *dest = htonl(num);
}

void ArchiveDownload::Omny::FlvEncoder::write_uint24(uint32_t num) {
    uint32_t nnum = htonl(num);
    if (htons(0x0001) != 0x0001) {
        nnum >>= 8;
    }
    for (uint8_t i=0; i < 3; ++i) {
        buffer->push_back(static_cast<uint8_t>((nnum & (0x000000ffu << i*8)) >> i*8));
    }
}

void ArchiveDownload::Omny::FlvEncoder::write_uint16(uint16_t num) {
    buffer->resize(buffer->size() + 2);
    uint16_t* dest = reinterpret_cast<uint16_t*>(&(*buffer)[buffer->size() - 2]);
    *dest = htons(num);
}

void ArchiveDownload::Omny::FlvEncoder::write_header(bool has_video, bool has_audio) {
    buffer->push_back(0x46);	// F
    buffer->push_back(0x4c);	// L
    buffer->push_back(0x56);	// V
    buffer->push_back(0x01);	// Version = 1
    uint8_t flags = 0;
    if (has_video) flags |= 0x01;
    if (has_audio) flags |= 0x04;
    buffer->push_back(flags);
    const uint32_t header_offset = 9;
    write_uint32(header_offset);
    prev_frame_size = 0;
}

void ArchiveDownload::Omny::FlvEncoder::write_frame_header(TagType type,
    uint32_t data_size, uint32_t timestamp) {

    buffer->push_back(static_cast<uint8_t>(type));  // TagType
    write_uint24(data_size);  // DataSize
    write_uint24(timestamp);	// Timestamp
    buffer->push_back(static_cast<uint8_t>((timestamp & 0xff000000u) >> 24));  // TimestampExt
    write_uint24(0);  // StreamID = 0
}

void ArchiveDownload::Omny::FlvEncoder::write_videodata_header(AVCPacketType packet_type, bool keyFrame) {
    uint8_t frame_type_and_codec_id = 0x07; // CodecID = AVC
    frame_type_and_codec_id |= ((keyFrame ? 1 : 2) << 4); // 0x10 - keyframe, 0x20 - interframe
    buffer->push_back(frame_type_and_codec_id);
    buffer->push_back(static_cast<uint8_t>(packet_type)); // AVCPacketType
    write_uint24(0); // CompositionTime
}

void ArchiveDownload::Omny::FlvEncoder::write_script_string(std::string const& str) {
    uint16_t length = static_cast<uint16_t>(str.size());
    write_uint16(length);
    if (length) {
        uint8_t* dest = &(*buffer)[0] + buffer->size();
        buffer->resize(buffer->size() + length);
        memcpy(dest, str.data(), length);
    }
}

void ArchiveDownload::Omny::FlvEncoder::write_script_number(double num) {
    buffer->push_back(0x00);  // type - double
    uint8_t* numPtr = reinterpret_cast<uint8_t*>(&num);
    bool host_is_le = htons(0x0001) != 0x0001;
    for (int i=0; i < 8; ++i) {
        buffer->push_back(numPtr[host_is_le ? 7-i : i]);
    }
}

void ArchiveDownload::Omny::FlvEncoder::write_script_frame(FileHeader const& file_header) {
    write_uint32(prev_frame_size);
    size_t data_size_write_pos = buffer->size() + 1;
    write_frame_header(ttScriptData, 0, 0);

    typedef std::pair<std::string, double> ScriptParamNumber;
    typedef std::pair<std::string, std::string> ScriptParamString;

    static const std::string caption("onMetaData");
    static const int num_params_count = 7;
    static ScriptParamNumber numParams[num_params_count] = {
        ScriptParamNumber("duration", 0),
        ScriptParamNumber("width", 0),
        ScriptParamNumber("height", 0),
        ScriptParamNumber("videodatarate", 125),
        ScriptParamNumber("framerate", 1000),
        ScriptParamNumber("videocodecid", 7),
        ScriptParamNumber("filesize", 0)
    };
    static const ScriptParamString encoder("encoder", "Lavf55.19.104");

    numParams[1].second = file_header.video.width;
    numParams[2].second = file_header.video.height;

    size_t begin_pos = buffer->size();
    buffer->push_back(0x02);  // type - string
    write_script_string(caption);
    buffer->push_back(0x08);  // type - array
    write_uint32(num_params_count + 1);  // +1 string param
    for (int i = 0; i < num_params_count; ++i) {
        write_script_string(numParams[i].first);
        write_script_number(numParams[i].second);
    }
    write_script_string(encoder.first);
    buffer->push_back(0x02);  // type - string
    write_script_string(encoder.second);
    // write end
    buffer->push_back(0x00);
    buffer->push_back(0x00);
    buffer->push_back(0x09);

    uint32_t data_size = static_cast<uint32_t>(buffer->size() - begin_pos);
    uint32_t ndata_size = htonl(data_size);
    if (htons(0x0001) != 0x0001) {
        ndata_size >>= 8;
    }
    for (uint8_t i = 0; i < 3; ++i) {
        (*buffer)[data_size_write_pos + i] = static_cast<uint8_t>((ndata_size & (0x000000ffu << i*8)) >> i*8);
    }
    prev_frame_size = data_size + frame_header_offset;
}

void ArchiveDownload::Omny::FlvEncoder::write_frame(FrameShp frame) {
    if (frame->header.type == 0) return;
    write_uint32(prev_frame_size);
    uint32_t data_size = video_data_offset + frame->header.size;
    write_frame_header(ttVideo, data_size, static_cast<uint32_t>(frame->ts));

    // VideoData:
    write_videodata_header(aptNalu, frame->header.type == 1);
    prev_frame_size = data_size + frame_header_offset;
}

void ArchiveDownload::Omny::FlvEncoder::write_init_frame(uint32_t timestamp) {
    write_uint32(prev_frame_size);
    uint32_t data_size = video_data_offset + avc_decoder_config_offset;
    if (!sps.empty()) {
        data_size += 2 + sps.size();  // 2 byte sps size
    }
    data_size += 1; // Num of pps
    if (!pps.empty()) {
        data_size += 2 + pps.size();  // 2 byte pps size
    }
    write_frame_header(ttVideo, data_size, timestamp);
    write_videodata_header(aptSequenceHeader, true);

    // AvcDecoderConfigurationRecord:
    buffer->push_back(1);  // Version
    buffer->push_back(avcpMainProfile);  // AvcProfileIndication
    buffer->push_back(0);  // profile_compatibility
    buffer->push_back(avcl4_2);  // AvcLevelIndication
    uint8_t one_byte = 0xfc; // b11111100 - 6 bits reserved
    uint8_t nalu_size_minus_one = 3;
    one_byte |= 0x03 & nalu_size_minus_one;
    buffer->push_back(one_byte);

    one_byte = 0xe0 | 0x01; // b11100000 - 3 bits reserved + num_of_sps = 1
    buffer->push_back(one_byte);
    write_uint16(static_cast<uint16_t>(sps.size()));  // sps length
    if (!sps.empty()) {
        uint8_t* dest = &(*buffer)[0] + buffer->size();
        buffer->resize(buffer->size() + sps.size());
        memcpy(dest, &sps[0], sps.size());
    }
    buffer->push_back(1);  // num_of_pps = 1
    write_uint16(static_cast<uint16_t>(pps.size()));  // pps length
    if (!pps.empty()) {
        uint8_t* dest = &(*buffer)[0] + buffer->size();
        buffer->resize(buffer->size() + pps.size());
        memcpy(dest, &pps[0], pps.size());
    }
    prev_frame_size = data_size + frame_header_offset;
}

void ArchiveDownload::Omny::FlvEncoder::write_final_frame(uint32_t timestamp) {
    write_uint32(prev_frame_size);
    write_frame_header(ttVideo, video_data_offset, timestamp);
    write_videodata_header(aptEndOfSequence, true);
    prev_frame_size = video_data_offset + frame_header_offset;
}
