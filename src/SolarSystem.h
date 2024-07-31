//
// Created by Patrick on 31/07/2024.
//

#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H
#include <functional>
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "CelestialBody.h"
#include "PointLight.h"


class SolarSystem {
public:
    explicit SolarSystem(const std::string &meshName) {
        loadMesh(meshName);
        loadBodies();
    }

    [[nodiscard]] PointLight getLight() const {
        return light;
    }

    CelestialBody *getBody(const int idx) {
        return &bodies[idx];
    }

    size_t getNumBodies() {
        return bodies.size();
    }

private:
    GeometryBuffer buf;
    PointLight light;
    std::vector<CelestialBody> bodies;

    void loadBodies() {
        CelestialBodyInfo sunInfo{-0.6, 0, 0, 1.0, true};
        bodies.emplace_back(sunInfo, &buf, "2k_sun.jpg");
        this->light = PointLight(
            {0, 0, 0},
            {1, 1, 1},
            20.0,
            {1.0, 0.09, 0.32}
        );

        CelestialBodyInfo mercuryInfo{-0.255754475703324, -0.170512674775491, 1, 0.15};
        bodies.emplace_back(mercuryInfo, &buf, "2k_mercury.jpg");

        CelestialBodyInfo venusInfo{0.061723632435768, -0.066755674232309, 2, 0.19};
        bodies.emplace_back(venusInfo, &buf, "2k_venus.jpg");

        CelestialBodyInfo earthInfo{-15.04178272980501, -0.041067761806981, 4, 0.2};
        bodies.emplace_back(earthInfo, &buf, "2k_earth.jpg");

        CelestialBodyInfo marsInfo{-14.62423832092078, -0.021834696788843, 6, 0.5};
        bodies.emplace_back(marsInfo, &buf, "2k_mars.jpg");

        CelestialBodyInfo jupiterInfo{-36.30252100840336, -0.003469957111330, 8, 0.7};
        bodies.emplace_back(jupiterInfo, &buf, "2k_jupiter.jpg");

        CelestialBodyInfo saturnInfo{-33.38485316846986, -0.001394609369915, 10, 0.6};
        bodies.emplace_back(saturnInfo, &buf, "2k_saturn.jpg");

        CelestialBodyInfo uranusInfo{20.88974854932301, -0.000488803945625, 12, 0.4};
        bodies.emplace_back(uranusInfo, &buf, "2k_uranus.jpg");

        CelestialBodyInfo neptuneInfo{-22.36024844720496, -0.000249210708152, 14, 0.35};
        bodies.emplace_back(neptuneInfo, &buf, "2k_neptune.jpg");
    }

    void loadMesh(const std::string &meshName) {
        std::filesystem::path path(ROOT_DIR "res/" + meshName);
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path.string(),
                                                 aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                                                 aiProcess_JoinIdenticalVertices |
                                                 aiProcess_SortByPType | aiProcess_PreTransformVertices);
        if (!scene) {
            std::cerr << "Importing of 3D scene failed: " << importer.GetErrorString() << std::endl;
            return;
        }
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            std::vector<float> vertices;

            aiMesh *mesh = scene->mMeshes[i];
            for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
                auto vertex = mesh->mVertices[j];
                vertices.push_back(vertex.x);
                vertices.push_back(vertex.y);
                vertices.push_back(vertex.z);

                auto normal = mesh->mNormals[j];
                vertices.push_back(normal.x);
                vertices.push_back(normal.y);
                vertices.push_back(normal.z);

                auto texCoord = mesh->mTextureCoords[0][j];
                vertices.push_back(texCoord.x);
                vertices.push_back(texCoord.y);
            }
            std::vector<uint32_t> indices;
            for (unsigned int k = 0; k < mesh->mNumFaces; k++) {
                aiFace &face = mesh->mFaces[k];
                for (unsigned int l = 0; l < face.mNumIndices; l++) {
                    indices.push_back(face.mIndices[l]);
                }
            }
            buf.setVBOData(vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
            buf.setVAOData(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
            buf.setVAOData(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                           reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
            buf.setVAOData(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                           reinterpret_cast<GLvoid *>(6 * sizeof(GLfloat)));
            buf.setEBOData(indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW, indices.size());
        }
    }
};


#endif //SOLARSYSTEM_H
