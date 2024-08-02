//
// Created by Patrick on 02/08/2024.
//

#ifndef CAMERA_H
#define CAMERA_H
#include "CelestialBody.h"
#include <glm/glm.hpp>

enum class ProjectionType {
    PERSPECTIVE,
    ORTHOGRAPHIC
};

class Camera {
public:
    Camera(const glm::vec3 &pos, const glm::fvec2 angles, float fov, float aspect_ratio, CelestialBody *focused_body,
           ProjectionType projection_type = ProjectionType::PERSPECTIVE
    )
        : projectionType(projection_type),
          pos(pos),
          angles(angles),
          fov(fov),
          aspectRatio(aspect_ratio),
          focusedBody(focused_body) {
        updateProjection();
    }

    [[nodiscard]] ProjectionType getProjectionType() const {
        return projectionType;
    }


    [[nodiscard]] float getFieldOfView() const {
        return fov;
    }

    [[nodiscard]] glm::mat4 getProjection() const {
        return projectionType == ProjectionType::PERSPECTIVE
                   ? perspective
                   : orthographic;
    }

    void setAspectRatio(const float newAspectRatio) {
        aspectRatio = newAspectRatio;
        updateProjection();
    }

    void changeFieldOfView(const float change) {
        fov = fov + change;
        fov = std::clamp(fov, 5.0f, 120.0f);
        updateProjection();
    }

    [[nodiscard]] glm::fvec2 getAngles() const {
        return angles;
    }

    [[nodiscard]] glm::mat4 calcView(const double simTime) const {
        auto view = translate(glm::mat4(1.0f), -pos);
        view = rotate(view, glm::radians(angles.y), glm::vec3(1.0f, 0.0f, 0.0f));
        view = rotate(view, glm::radians(angles.x), glm::vec3(0.0f, 1.0f, 0.0f));
        view = translate(view, -focusedBody->getPosition(simTime));

        return view;
    }

    [[nodiscard]] glm::vec3 getPos() const {
        return pos;
    }

    void setPos(const glm::vec3 &pos) {
        this->pos = pos;
    }

    void setAngles(const glm::fvec2 &angles) {
        this->angles = angles;
        this->angles.y = std::clamp(angles.y, -89.0f, 89.0f);
    }

    void setProjectionType(ProjectionType projectionType) {
        this->projectionType = projectionType;
    }

    void toggleProjectionType() {
        projectionType = projectionType == ProjectionType::PERSPECTIVE
                             ? ProjectionType::ORTHOGRAPHIC
                             : ProjectionType::PERSPECTIVE;
    }

    void setFocusedBody(CelestialBody *focusedBody) {
        this->focusedBody = focusedBody;
    }

private:
    ProjectionType projectionType;
    glm::mat4 perspective{};
    glm::mat4 orthographic{};
    glm::vec3 pos;
    glm::fvec2 angles;
    float fov;
    float aspectRatio;
    glm::dvec2 lastMousePos{};
    CelestialBody *focusedBody;

    void updateProjection() {
        perspective = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);

        float xSpan = 8 * (fov / 45.0f);
        float ySpan = 8 * (fov / 45.0f);

        if (aspectRatio > 1) {
            xSpan *= aspectRatio;
        } else {
            ySpan = ySpan / aspectRatio;
        }
        orthographic = glm::ortho(-xSpan, xSpan, -ySpan, ySpan, 0.1f, 1000.0f);
    }
};


#endif //CAMERA_H
