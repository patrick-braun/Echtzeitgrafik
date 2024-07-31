//
// Created by Patrick on 31/07/2024.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <filesystem>
#include <iostream>
#include <GL/glew.h>

#include "stb_image.h"
#include "helper/RootDir.h"

class Texture {
public:
    explicit Texture(const std::string &textureName) {
        int width, height, nrChannels;
        std::filesystem::path path(ROOT_DIR "res/textures/" + textureName);
        unsigned char *imageData = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);
        if (!imageData) {
            std::cerr << "Failed to load texture!" << std::endl;
        }
        std::cout << width << " " << height << " " << nrChannels << std::endl;
        glGenTextures(1, &glTexture);
        glBindTexture(GL_TEXTURE_2D, glTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(imageData);
    }

    ~Texture() {
        glDeleteTextures(1, &glTexture);
    }

    Texture(const Texture &other) = delete;

    Texture(Texture &&other) noexcept
        : glTexture(other.glTexture) {
        other.glTexture = 0;
    }

    Texture &operator=(const Texture &other) = delete;

    Texture &operator=(Texture &&other) noexcept {
        if (this == &other)
            return *this;
        glTexture = other.glTexture;
        other.glTexture = 0;
        return *this;
    }

    [[nodiscard]] GLuint getGlTexture() const {
        return glTexture;
    }

    void bind() {
        glBindTexture(GL_TEXTURE_2D, glTexture);
    }

    void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    GLuint glTexture{};
};


#endif //TEXTURE_H
