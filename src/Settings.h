//
// Created by Patrick on 30/07/2024.
//

#ifndef SETTINGS_H
#define SETTINGS_H
#include "CelestialBody.h"
#include "Camera.h"

class Settings {
public:
    explicit Settings(Camera cam, float speed = 7): cam(cam), speed(speed) {
    }

    [[nodiscard]] float getSpeed() const {
        return speed;
    }

    void setSpeed(float speed) {
        this->speed = speed;
    }

    void doubleSpeed() {
        speed *= 2;
        speed = std::clamp(speed, 0.1f, 2048.0f);
    }

    void halfSpeed() {
        speed /= 2;
        speed = std::clamp(speed, 0.1f, 2048.0f);
    }

    void togglePause() {
        paused = !paused;
    }

    [[nodiscard]] bool isPaused() const {
        return paused;
    }

    Camera *getCamera() {
        return &cam;
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

        offset += cam.getAngles();
        cam.setAngles(offset);
    }

private:
    Camera cam;
    glm::dvec2 lastMousePos = {0.0, 0.0};
    bool paused = false;
    float speed = 1;
};


#endif //SETTINGS_H
