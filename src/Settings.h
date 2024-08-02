//
// Created by Patrick on 30/07/2024.
//

#ifndef SETTINGS_H
#define SETTINGS_H

enum class ProjectionType {
    PERSPECTIVE,
    ORTHOGRAPHIC
};

struct CameraInfo {
    glm::vec3 pos;
    glm::fvec2 angles;
    float fov;
    float aspectRatio;
};


class Settings {
public:
    explicit Settings(const CameraInfo &cam, ProjectionType projection_type = ProjectionType::PERSPECTIVE,
                      int speed = 7)
        : projectionType(projection_type),
          speed(speed) {
        this->cam = cam;
        updateProjection();
        updateView();
    }


    void setProjectionType(ProjectionType projectionType) {
        this->projectionType = projectionType;
    }

    void toggleProjectionType() {
        projectionType = projectionType == ProjectionType::PERSPECTIVE
                             ? ProjectionType::ORTHOGRAPHIC
                             : ProjectionType::PERSPECTIVE;
    }

    void togglePause() {
        paused = !paused;
    }

    [[nodiscard]] bool isPaused() const {
        return paused;
    }

    [[nodiscard]] ProjectionType getProjectionType() const {
        return projectionType;
    }

    [[nodiscard]] int getSpeed() const {
        return speed;
    }

    void setSpeed(int speed) {
        this->speed = speed;
    }

    void doubleSpeed() {
        speed *= 2;
        speed = std::clamp(speed, 1, 2048);
    }

    void halfSpeed() {
        speed /= 2;
        speed = std::clamp(speed, 1, 2048);
    }

    [[nodiscard]] float getFieldOfView() const {
        return cam.fov;
    }

    [[nodiscard]] glm::mat4 getProjection() const {
        return projectionType == ProjectionType::PERSPECTIVE
                   ? perspective
                   : orthographic;
    }

    void setAspectRatio(const float newAspectRatio) {
        cam.aspectRatio = newAspectRatio;
        updateProjection();
    }

    void changeFieldOfView(const float change) {
        cam.fov = cam.fov + change;
        cam.fov = std::clamp(cam.fov, 10.0f, 120.0f);
        updateProjection();
    }

    CameraInfo &getCameraInfo() {
        return cam;
    }

    [[nodiscard]] glm::mat4 getView() const {
        return view;
    }

    void setCamPos(const glm::vec3 &pos) {
        cam.pos = pos;
        updateView();
    }

    void setCamAngles(const glm::fvec2 &angles) {
        cam.angles = angles;
        cam.angles.y = std::clamp(angles.y, -89.0f, 89.0f);
        updateView();
    }

    void setLastMousePos(double xPos, double yPos) {
        lastMousePos = glm::dvec2(xPos, yPos);
    }

    void updateMouse(double xPos, double yPos) {
        if (lastMousePos == glm::dvec2(0.0, 0.0)) {
            lastMousePos = glm::dvec2(xPos, yPos);
            return;
        }
        auto offset = glm::dvec2(xPos, yPos) - lastMousePos;
        lastMousePos = glm::dvec2(xPos, yPos);
        offset *= 0.1;

        offset += cam.angles;
        setCamAngles(offset);
    }

private:
    ProjectionType projectionType;
    bool paused = false;
    int speed = 1;
    glm::mat4 perspective{};
    glm::mat4 orthographic{};
    glm::mat4 view{};
    CameraInfo cam{};
    glm::dvec2 lastMousePos{};

    void updateProjection() {
        perspective = glm::perspective(glm::radians(cam.fov), cam.aspectRatio, 0.1f, 1000.0f);

        float xSpan = 8 * (cam.fov / 45.0f);
        float ySpan = 8 * (cam.fov / 45.0f);

        if (cam.aspectRatio > 1) {
            xSpan *= cam.aspectRatio;
        } else {
            ySpan = ySpan / cam.aspectRatio;
        }
        orthographic = glm::ortho(-xSpan, xSpan, -ySpan, ySpan, 0.1f, 1000.0f);
    }

    void updateView() {
        view = translate(glm::mat4(1.0f), -cam.pos);
        view = glm::rotate(view, glm::radians(cam.angles.y), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(cam.angles.x), glm::vec3(0.0f, 1.0f, 0.0f));
    }
};


#endif //SETTINGS_H
