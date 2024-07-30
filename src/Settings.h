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

    [[nodiscard]] ProjectionType getProjectionType() const {
        return projectionType;
    }

private:
    ProjectionType projectionType;
};


#endif //SETTINGS_H
