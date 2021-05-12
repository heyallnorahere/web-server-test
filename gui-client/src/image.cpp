#include "image.h"
#include <glad/glad.h>
#include <cassert>
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
        int internal_format;
        switch (channels) {
        case 1:
            internal_format = GL_RED;
            break;
        case 2:
            internal_format = GL_RG;
            break;
        case 3:
            internal_format = GL_RGB;
            break;
        case 4:
            internal_format = GL_RGBA;
            break;
        default:
            assert(false);
            break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}