//
// Created by Patrick on 31/07/2024.
//

#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H
#include "GeometryBuffer.h"
#include "Texture.h"


struct CelestialBodyInfo {
    double rotationDegreesPerHour;
    double orbitDegreesPerHour;
    double distanceFromParent;
};

class CelestialBody {
public:
    CelestialBody(const CelestialBodyInfo &info, GeometryBuffer &geometry_buffer, const std::string &textureName)
        : info(info),
          geometryBuffer(geometry_buffer),
          texture(textureName) {
    }

private:
    CelestialBodyInfo info;
    GeometryBuffer &geometryBuffer;
    Texture texture;
};



#endif //CELESTIALBODY_H
