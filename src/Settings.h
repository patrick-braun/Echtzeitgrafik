//
// Created by Patrick on 30/07/2024.
//

#ifndef SETTINGS_H
#define SETTINGS_H

enum class ProjectionType {
    PERSPECTIVE,
    ORTHOGRAPHIC
};


class Settings {
public:
    Settings() : projectionType(ProjectionType::PERSPECTIVE) {
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
        fieldOfView = fieldOfView + change;
        fieldOfView = std::clamp(fieldOfView, 10.0f, 120.0f);
        updateProjection();
    }

private:
    ProjectionType projectionType;
    bool paused = false;
    int speed = 7;
    float fieldOfView = 45.0f;
    float aspectRatio = 8.0f / 6.0f;
    glm::mat4 perspective = glm::perspective(glm::radians(fieldOfView), 800.0f / 600.0f, 0.1f, 1000.0f);
    glm::mat4 orthographic = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 1000.0f);

    void updateProjection() {
        perspective = glm::perspective(glm::radians(fieldOfView), aspectRatio, 0.1f, 1000.0f);

        float xSpan = 8 * (fieldOfView / 45.0f);
        float ySpan = 8 * (fieldOfView / 45.0f);

        if (aspectRatio > 1){
            xSpan *= aspectRatio;
        }
        else{
            ySpan = ySpan / aspectRatio;
        }
        orthographic = glm::ortho(-xSpan, xSpan, -ySpan, ySpan, 0.1f, 1000.0f);
    }
};


#endif //SETTINGS_H
