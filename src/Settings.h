//
// Created by Patrick on 30/07/2024.
//

#ifndef SETTINGS_H
#define SETTINGS_H
#include "CelestialBody.h"
#include "Camera.h"
#include "SolarSystem.h"

class Settings {
public:
    explicit Settings(Camera cam, SolarSystem *solarSystem, float speed = 8.0): cam(cam), solarSystem(solarSystem),
        speed(speed) {
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

    void setFocusedBody(const int idx) {
        const auto body = solarSystem->getBody(idx);
        cam.setFocusedBody(body);
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

    [[nodiscard]] glm::dvec2 getMousePosBeforeDrag() const {
        return mousePosBeforeDrag;
    }

    void setMousePosBeforeDrag(const glm::dvec2 &mousePosBeforeDrag) {
        this->mousePosBeforeDrag = mousePosBeforeDrag;
    }

    [[nodiscard]] bool getDragging() const {
        return dragging;
    }

    void setDragging(bool dragging) {
        this->dragging = dragging;
    }

private:
    Camera cam;
    SolarSystem *solarSystem;
    glm::dvec2 lastMousePos = {0.0, 0.0};
    glm::dvec2 mousePosBeforeDrag = {0.0, 0.0};
    bool dragging = false;
    bool paused = false;
    float speed = 1;
};


#endif //SETTINGS_H
