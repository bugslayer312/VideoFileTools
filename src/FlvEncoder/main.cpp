#include "Encoder.h"

using namespace ArchiveDownload::Omny;

#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

int main(int argc, char* argv[]) {
    FrameShp frame;
    if (argc > 1) {
        std::vector<char> v;
        v.reserve(65536);
        char buff[4096];
        std::ifstream ifs(argv[1], std::ios::in|std::ios::binary);
        if (!ifs) {
            std::cerr << "Failed to open " << argv[1] << std::endl;
            return 1;
        }
        while (ifs.read(buff, sizeof(buff))) {
            size_t prev_size = v.size();
            v.resize(prev_size + 4096);
            memcpy(&v[0]+prev_size, buff, 4096);
        }
        if (size_t last_read_count = ifs.gcount()) {
            size_t prev_size = v.size();
            v.resize(prev_size + last_read_count);
            memcpy(&v[0]+prev_size, buff, last_read_count);
        }
        frame.reset(new ArchiveDownload::Omny::Frame());
        frame->header.type = 1;
        frame->header.size = v.size();
        frame->buffer.resize(frame->header.size);
        memcpy(frame->buffer.data, &v[0], frame->header.size);
        frame->ts = 0;
    }
    FlvEncoder enc;
    enc.reset_buffer(512);
    enc.write_header(true, false);
    FileHeader fh;
    fh.video.width = 1920;
    fh.video.height = 1280;
    enc.write_script_frame(fh);
    if (frame) {
        ArchiveDownload::Omny::Reader::fix_frame_data(frame->buffer.data, frame->header.size);
        enc.load_sps_pps_from_frame(frame);
    }
    if (enc.sps.empty()) {
        enc.create_default_sps();
    }
    if (enc.pps.empty()) {
        enc.create_default_pps();
    }
    enc.write_init_frame(40);

    std::ofstream ofs("/home/eugene/Projects/VideoFileTools/data/test.flv",
                      std::ios::out|std::ios::binary|std::ios::ate);

    if (frame) {
        enc.write_frame(frame);
        ofs.write(reinterpret_cast<char*>(&(*enc.buffer)[0]), static_cast<long>(enc.buffer->size()));
        enc.reset_buffer(64);
        ofs.write(frame->buffer.data, frame->header.size);
    }
    enc.write_final_frame(20);
    enc.write_uint32(enc.prev_frame_size);
    ofs.write(reinterpret_cast<char*>(&(*enc.buffer)[0]), static_cast<long>(enc.buffer->size()));
    return 0;
}
