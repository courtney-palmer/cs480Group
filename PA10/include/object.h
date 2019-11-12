#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

const std::string NA("NA"); // Constant to define n/a text file name

class Object
{
  public:
    Object(const std::string& objFileName,      const ShapeInfo& newShape,
	   const std::string& key = NA, const std::string& texFileName = NA);
    ~Object();

    void Update();
    void Render();
    void showMeshData() const;

    glm::mat4 GetModel();
    glm::mat4 model;
    
    std::string getKeyname() const { return key; }

  private:
    std::string keyname;

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
