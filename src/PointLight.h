//
// Created by Patrick on 30/07/2024.
//

#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include <glm/vec3.hpp>

struct attenuation {
    float constant;
    float linear;
    float quadratic;
};


class PointLight {
public:
    PointLight() = default;

    PointLight(glm::vec3 position, glm::vec3 color, float intensity,
               attenuation attenuation_components) : position(position),
                                                     color(color), intensity(intensity),
                                                     attenuationComponents(attenuation_components) {
    }

    [[nodiscard]] const glm::vec3 &getPosition() const {
        return position;
    }

    [[nodiscard]] const glm::vec3 &getColor() const {
        return color;
    }

    [[nodiscard]] float getIntensity() const {
        return intensity;
    }

    [[nodiscard]] attenuation getAttenuation() const {
        return attenuationComponents;
    }

    void setPosition(const glm::vec3 &position) {
        this->position = position;
    }

    void setColor(const glm::vec3 &color) {
        this->color = color;
    }

    void setIntensity(float intensity) {
        this->intensity = intensity;
    }

    void setAttenuation(const attenuation &attenuation) {
        this->attenuationComponents = attenuation;
    }

private:
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    attenuation attenuationComponents;
};


#endif //POINTLIGHT_H
