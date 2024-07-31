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

private:
    ProjectionType projectionType;
    bool paused = false;
    int speed = 7;
};


#endif //SETTINGS_H
