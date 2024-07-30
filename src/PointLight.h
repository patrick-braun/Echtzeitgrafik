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
    PointLight(glm::vec3 position, glm::vec3 color, float intensity,
               attenuation attenuation_components) : position(position),
                                                     color(color), intensity(intensity),
                                                     attenuation_components(attenuation_components) {
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
        return attenuation_components;
    }

private:
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    attenuation attenuation_components;
};


#endif //POINTLIGHT_H
