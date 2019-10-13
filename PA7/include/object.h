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

    // Constructor for planet w/ all values
    //      string key, modelfile, texturefile, origin;
    //  float scale, speed, rotationSpeed, orbitRadius;
    Object(std::string objname, std::string texturename,
	   std::string key, int origin,
	   float scale, float speed, float rotSpeed, float orbRadius);

    ~Object();
    void Update(unsigned int dt, glm::mat4 orbitOrigin);
    //void Update(unsigned int dt);
    void Render();

    glm::mat4 GetModel();
    glm::mat4 GetPosition();
    std::string getKey();
    int getOriginIndex();
    void DisplayModelInfo(const unsigned int maxDisplayLines = 10);
    //void SetScale(bool scalar); //if scalar true, increase, else decrease
    void SetOrbitSpeed(bool scalar); //if scalar true, increase, else decrease
    void SetSpinSpeed(bool scalar); //if scalar true, increase, else decrease
    
    bool loadModel(std::string objFileName);
    bool loadTexture(std::string textFileName);

  private:
    std::string keyname;
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
    int origin; //index of planet that it orbits, for the sun this value is -1

    std::vector<meshInfo> meshData;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint *texture;
    //std::vector<string> textureList;

    float angleOrbit; //the angle of the orbit
    float angleSelf; //the angle of the self rotation
    glm::mat4 position; //holds the position of the object

    float baseScale; //the default scale of the object
    //float scaleMult; //the multiplier of the object's scale
    //float maxScale; //the maximum multiplier that the object can scale to
    //float minScale; //the minimum multiplier that the object can scale to

    float baseOrbitSpeed; //the default orbit speed of the object
    float baseSpinSpeed; //the default spin speed of the object
    float orbitSpeedMult; //the multiplier of the object's orbit speed
    float spinSpeedMult; //the multiplier of the object's spin speed
    float maxSpeed; //the maximum multiplier of the object's orbit/spin speed
    float minSpeed; //the minimum multiplier of the object's orbit/spin speed

    float orbitDistance; // Distance the object rotates from its origin
	
    bool pausedOrbit; //determines if the orbit is paused
    bool pausedSpin; //determines if the spin is paused
    bool reversedOrbit; //determines if the orbit is reversed
    bool reversedSpin; //determines if the spin is reversed
};

#endif /* OBJECT_H */
