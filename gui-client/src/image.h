#pragma once
#include <cstdint>
#include <vector>
namespace guifrontend {
    class image {
    public:
        image(const std::vector<uint8_t>& data, int width, int height, int channels);
        ~image();
        void* get_texture_id();
    private:
        uint32_t m_id;
        void setup(const std::vector<uint8_t>& data, int width, int height, int channels);
    };
}