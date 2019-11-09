#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

const std::string NA("NA");

class Object
{
  public:
  Object(std::string objFileName, const ShapeInfo& newShape,
	 std::string texFileName = NA);
    ~Object();
    void Update();
    void Render();
    void showMeshData() const;

    glm::mat4 GetModel();
    glm::mat4 model;
    
  private:
    unsigned int meshNumber;
    Assimp::Importer importer;
    const aiScene* scene;

    struct meshInfo {
      unsigned int meshSize, meshStartIndex;
    meshInfo(unsigned int a, unsigned int b) : meshSize(a), meshStartIndex(b) {}
    };
    std::vector<meshInfo> meshData;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint* texture;

    bool loadModel(std::string objFileName);
    bool loadTexture(std::string textFileName);
    std::vector<std::string> textureNames;
    bool textured;

    //btVector3 velocity;


 public: // PUblic for testing reasons currently
    // Used for physics
    btCollisionObject *physicsObject; // Holds both world transform & shape, see shape
    btCollisionShape *shape; // Defines shape of collision object
    btTriangleMesh *objTriMesh; // used if shape is mesh, otherwise null
    Shape shapeType;
    btRigidBody *RBody;
    // for INLINE Getters & setters
    //btCollisionObject getCollisionObject() {return *physicsObject;}
    //btCollisionShape getCollisionShape() {return *shape;}

    bool isTextured() const { return textured; }
};

#endif /* OBJECT_H */
