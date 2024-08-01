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

    void updateProjection(const int width, const int height) {
        const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        perspective = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);

        float xSpan = 8;
        float ySpan = 8;

        if (aspectRatio > 1){
            xSpan *= aspectRatio;
        }
        else{
            ySpan = ySpan / aspectRatio;
        }
        orthographic = glm::ortho(-xSpan, xSpan, -ySpan, ySpan, 0.1f, 1000.0f);
    }

private:
    ProjectionType projectionType;
    bool paused = false;
    int speed = 7;
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
    glm::mat4 orthographic = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 1000.0f);
};


#endif //SETTINGS_H
