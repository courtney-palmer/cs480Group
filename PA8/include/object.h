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
  Object(std::string objFileName, Shape colliShape);
    ~Object();
    void Update(unsigned int dt);
    void Render();

    glm::mat4 GetModel();


  private:
    unsigned int meshNumber;
    Assimp::Importer importer;
    const aiScene* scene;

    glm::mat4 model;
    float angle;

    struct meshInfo {
      unsigned int meshSize, meshStartIndex;
    meshInfo(unsigned int a, unsigned int b) : meshSize(a), meshStartIndex(b) {}
    };
    std::vector<meshInfo> meshData;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

 public: // PUblic for testing reasons currently
    // Used for physics
    btCollisionObject *physicsObject; // Holds both world transform & shape, see shape
    btCollisionShape *shape; // Defines shape of collision object
    Shape shapeType;

    // for INLINE Getters & setters
    //btCollisionObject getCollisionObject() {return *physicsObject;}
    //btCollisionShape getCollisionShape() {return *shape;}
};

#endif /* OBJECT_H */
