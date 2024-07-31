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


class SolarSystem {
    SolarSystem(const std::string &meshName) {
        loadMesh(meshName);

    }

private:
    std::vector<CelestialBody> bodies;
    void loadMesh(const std::string &meshName) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(meshName,
                                                 aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                                                 aiProcess_JoinIdenticalVertices |
                                                 aiProcess_SortByPType | aiProcess_PreTransformVertices);
        if (!scene) {
            std::cerr << "Importing of 3D scene failed: " << importer.GetErrorString() << std::endl;
        }
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            std::vector<float> vertices;
            aiMesh* mesh = scene->mMeshes[i];
            for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
                // TODO: Copy the vertex data into the vector "vertices"
            }
            std::vector<uint32_t> indices;
            for (unsigned int k = 0; k < mesh->mNumFaces; k++) {
                aiFace& face = mesh->mFaces[k];
                // TODO: Copy the index values to the vector "indices" above
                // We can assume that there are only 3 indices per face
                // as we set aiProcess_Triangulate for the import
            }
            // TODO: Create Mesh and save it as member of scene
        }
        if (scene->HasLights()) {
            const aiLight *light = scene->mLights[0]; // Assume there is only one light in the scene
            switch (light->mType) {
                case aiLightSourceType::aiLightSource_DIRECTIONAL:
                    break;
                case aiLightSourceType::aiLightSource_POINT:

                    break;
                default: break;
            }
        }
    }
};


#endif //SOLARSYSTEM_H
