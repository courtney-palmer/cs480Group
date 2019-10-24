#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

class Object
{
  public:
    Object(char** argv);
    ~Object();
    bool loadModel(std::string objFileName);
    bool loadTexture(std::string textFileName);
    
    void Update(unsigned int dt);
    void Render();

    glm::mat4 GetModel();

  private:
    unsigned int meshNumber;
    Assimp::Importer importer;
    const aiScene* scene;
    glm::mat4 model;
    std::vector<std::string> textureNames;

    struct meshInfo {
      unsigned int meshSize, meshStartIndex;
    meshInfo(unsigned int a, unsigned int b) : meshSize(a), meshStartIndex(b) {}
    };
    std::vector<meshInfo> meshData;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint *texture;
};

#endif /* OBJECT_H */
