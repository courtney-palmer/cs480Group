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
    void Update();
    void Render();

    glm::mat4 GetModel();
    glm::mat4 model;
    
  private:
    unsigned int meshNumber;
    Assimp::Importer importer;
    const aiScene* scene;

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

    bool loadModel(std::string objFileName);
    void showMeshData() const;

 public: // PUblic for testing reasons currently
    // Used for physics
    btCollisionObject *physicsObject; // Holds both world transform & shape, see shape
    btCollisionShape *shape; // Defines shape of collision object
    Shape shapeType;
    btRigidBody *RBody;
    // for INLINE Getters & setters
    //btCollisionObject getCollisionObject() {return *physicsObject;}
    //btCollisionShape getCollisionShape() {return *shape;}
};

#endif /* OBJECT_H */
