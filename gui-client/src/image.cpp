#include "image.h"
#include <glad/glad.h>
#include <cassert>
#include <stddef.h> // size_t isnt defined on arch??????
namespace guifrontend {
    image::image(const std::vector<uint8_t>& data, int width, int height, int channels) {
        this->setup(data, width, height, channels);
    }
    image::~image() {
        glDeleteTextures(1, &this->m_id);
    }
    void* image::get_texture_id() {
        // hacky, but the only way to do it
        return (void*)(size_t)this->m_id;
    }
    void image::setup(const std::vector<uint8_t>& data, int width, int height, int channels) {
        glGenTextures(1, &this->m_id);
        glBindTexture(GL_TEXTURE_2D, this->m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int format;
        switch (channels) {
        case 1:
            format = GL_RED;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            assert(false);
            break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data.data());
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}