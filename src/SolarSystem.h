//
// Created by Patrick on 31/07/2024.
//

#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H
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

    [[nodiscard]] std::vector<CelestialBody>* getBodies() {
        return &bodies;
    }

private:
    GeometryBuffer buf;
    PointLight light;
    std::vector<CelestialBody> bodies;

    void loadBodies() {
        CelestialBodyInfo sunInfo{0.05, 0, 0};
        bodies.emplace_back(sunInfo, &buf, "2k_sun.jpg");
        this->light = PointLight(
            {0, 0, 0},
            {1, 1, 1},
            20.0,
            {1.0, 0.09, 0.32}
        );

        CelestialBodyInfo earthInfo{0.15, 0.05, 4};
        bodies.emplace_back(earthInfo, &buf, "2k_earth.jpg");
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
