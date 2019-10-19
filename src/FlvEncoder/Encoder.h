#pragma once

#include <stdint.h>
#include <memory>
#include <vector>

namespace SmartPointer {
    struct Buffer {
        char* data;
        Buffer() : data(0){}
        ~Buffer() {
            free(data);
        }
        void resize(uint32_t size) {
            if (data) free(data);
            if (size) data = (char*)malloc(size);
        }
    };
}

namespace ArchiveDownload {
    namespace Omny {
        namespace Reader {
            void fix_frame_data(char* data, size_t data_count);
        }
        struct FileHeader {
            int magic; //0x20151105
            unsigned char chno;
            unsigned char type; //bit0:timer bit1:alarm
            unsigned char stream; //0:main 1:sub
            unsigned char meta; //bit0:video bit1:audio
            int start; //second for local
            int off;
            struct {
                int codec; //0:h264 3:h265
                int width;
                int height;
            } video;
            struct {
                int codec; //0:g711u 1:g711a 2:aacl
                int samples;
                int channel;
            } audio;
            char resv[24];
            bool differ(const FileHeader &rhs) {
                if(meta != rhs.meta) return true;
                if(video.codec != rhs.video.codec
                   || video.width != rhs.video.width
                   || video.height != rhs.video.height) return true;
                if((meta & 2) && (audio.codec != rhs.audio.codec
                                  || audio.samples != rhs.audio.samples
                                  || audio.channel != rhs.audio.channel)) return true;
                return false;
            }
        };

        struct FrameHeader {
            unsigned int magic:24; //0x151110
            unsigned int type:8; //0:A 1:I 2:P
            unsigned int size;
            long long ts;
            unsigned short exmode;
            unsigned short exsize;
            unsigned char resv[4];
        };

        struct Frame {
            FrameHeader header;
            SmartPointer::Buffer buffer;
            long long ts;
        };
        typedef std::shared_ptr<Frame> FrameShp;

        struct FlvEncoder {
            enum TagType {
                ttAudio = 8,
                ttVideo = 9,
                ttScriptData = 18
            };
            enum AVCPacketType {
                aptSequenceHeader = 0,
                aptNalu = 1,
                aptEndOfSequence = 2
            };
            enum AVCProfile {
                avcpBaselineProfile = 66,
                avcpMainProfile = 77,
                avcpExtendedProfile = 88,
                avcpHighProfile = 100
            };
            enum AVCLevel {
                avcl3 = 30,
                avcl3_1 = 31,
                avcl3_2 = 32,
                avcl4 = 40,
                avcl4_1 = 41,
                avcl4_2 = 42
            };
            typedef std::shared_ptr<std::vector<uint8_t> > BufferShp;
            BufferShp buffer;
            std::vector<uint8_t> sps;
            std::vector<uint8_t> pps;
            uint32_t prev_frame_size;
            static const uint32_t frame_header_offset;
            static const uint32_t video_data_offset;
            static const uint32_t avc_decoder_config_offset;

            FlvEncoder();
            void load_sps_pps_from_frame(FrameShp frame);
            void create_default_sps();
            void create_default_pps();
            void reset_buffer(size_t size);
            void write_uint32(uint32_t num);
            void write_uint24(uint32_t num);
            void write_uint16(uint16_t num);
            void write_script_string(std::string const& str);
            void write_script_number(double num);
            void write_header(bool has_video, bool has_audio);
            void write_frame_header(TagType type, uint32_t data_size, uint32_t timestamp);
            void write_videodata_header(AVCPacketType packet_type, bool keyFrame);
            void write_frame(FrameShp frame);
            void write_script_frame(FileHeader const& file_header);
            void write_init_frame(uint32_t timestamp);
            void write_final_frame(uint32_t timestamp);
        };
    }
}
