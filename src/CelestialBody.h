//
// Created by Patrick on 31/07/2024.
//

#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GeometryBuffer.h"
#include "Texture.h"


struct CelestialBodyInfo {
    double rotationDegreesPerHour;
    double orbitDegreesPerHour;
    double distanceFromParent;
    float scaleFactor = 1.0;
    bool selfEmitting = false;
};

class CelestialBody {
public:
    CelestialBody(const CelestialBodyInfo &info, GeometryBuffer *geometryBuffer, const std::string &textureName)
        : info(info),
          geometryBuffer(geometryBuffer),
          texture(textureName) {
    }

    void render() {
        bind();
        glDrawElements(GL_TRIANGLES, geometryBuffer->getEBOSize(), GL_UNSIGNED_INT, nullptr);
        unbind();
    }

    void bind() {
        texture.bind();
        geometryBuffer->bindVAO();
    }

    void unbind() {
        texture.unbind();
        geometryBuffer->unbindVAO();
    }

    [[nodiscard]] glm::mat4 getTransformationMatrix(const double timeInHours) const {
        auto model = glm::mat4(1.0f);
        model = rotate(model, static_cast<float>(glm::radians(timeInHours * info.orbitDegreesPerHour)),
                       glm::vec3(0.0f, 1.0f, 0.0f));
        model = translate(model, glm::vec3(info.distanceFromParent, 0.0f, 0.0f));
        model = rotate(model, static_cast<float>(glm::radians(timeInHours * info.rotationDegreesPerHour)),
                       glm::vec3(0.0f, 1.0f, 0.0f));
        model = scale(model, glm::vec3(info.scaleFactor));
        return model;
    }

    [[nodiscard]] bool getSelfEmitting() const {
        return this->info.selfEmitting;
    }

    ~CelestialBody() = default;

    CelestialBody(const CelestialBody &other) = delete;

    CelestialBody(CelestialBody &&other) noexcept
        : info(other.info),
          geometryBuffer(other.geometryBuffer),
          texture(std::move(other.texture)) {
    }

    CelestialBody &operator=(const CelestialBody &other) = delete;

    CelestialBody &operator=(CelestialBody &&other) noexcept {
        if (this == &other)
            return *this;
        info = other.info;
        geometryBuffer = other.geometryBuffer;
        texture = std::move(other.texture);
        return *this;
    }

private:
    CelestialBodyInfo info;
    GeometryBuffer *geometryBuffer;
    Texture texture;
};


#endif //CELESTIALBODY_H
