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
    Object(float baseSc, float baseOS, float baseSS, char** argv);
    Object(float baseSc, float baseOS, float baseSS,
	   std::string objname, std::string texturename);
    ~Object();
    void Update(unsigned int dt, glm::mat4 orbitOrigin);
    void Render();

    glm::mat4 GetModel();
    glm::mat4 GetPosition();
    void DisplayModelInfo(const unsigned int maxDisplayLines = 10);
    void SetScale(bool scalar); //if scalar true, increase, else decrease
    void SetOrbitSpeed(bool scalar); //if scalar true, increase, else decrease
    void SetSpinSpeed(bool scalar); //if scalar true, increase, else decrease

    bool loadModel(std::string objFileName);
    bool loadTexture(std::string textFileName);

  private:
    std::vector<std::string> textureNames;

    unsigned int meshNumber;
    Assimp::Importer importer;
    const aiScene* scene;

    /* config info
       scale - scaleMult
       speed - orbitSpeedMult
       spinSpeed - spinSpeedMult
       orbitDistance - *needVar
       model - *need var - .obj file name
       mtl - name of mtl used
       prob to do:
       move vertex & index & texture loading into dedicated function for loading file info
     */
    glm::mat4 model;

    std::vector<meshInfo> meshData;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint *texture;
    //std::vector<string> textureList;

    float angleOrbit; //the angle of the orbit
    float angleSelf; //the anlgle of the self rotation
    glm::mat4 position; //holds the position of the object

    float baseScale; //the default scale of the object
    float scaleMult; //the multiplier of the object's scale
    float maxScale; //the maximum multiplier that the object can scale to
    float minScale; //the minimum multiplier that the object can scale to

    float baseOrbitSpeed; //the default orbit speed of the object
    float baseSpinSpeed; //the default spin speed of the object
    float orbitSpeedMult; //the multiplier of the object's orbit speed
    float spinSpeedMult; //the multiplier of the object's spin speed
    float maxSpeed; //the maximum multiplier of the object's orbit/spin speed
    float minSpeed; //the minimum multiplier of the object's orbit/spin speed
	
    bool pausedOrbit; //determines if the orbit is paused
    bool pausedSpin; //determines if the spin is paused
    bool reversedOrbit; //determines if the orbit is reversed
    bool reversedSpin; //determines if the spin is reversed
};

#endif /* OBJECT_H */
