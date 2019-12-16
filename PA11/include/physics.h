#ifndef PHYSICS_H
#define PHYSICS_H

#include "graphics_headers.h"
#include "object.h"
#include "sound.h"
#include <cmath>

class Engine;

class Physics
{
  public:
    Physics(Engine* e);
    ~Physics();
    bool Initialize();

    Engine* m_engine;

    void spawnDisk();
    void Update(std::vector<Object*>& objs, std::vector<Object*>& disks, unsigned int& score); // just used to stepSimulation and update positions
    //void Update(std::vector<Object*>& objs, unsigned int& score);
    
    // default : obj starts at origin as dynamic collision object
    // inserts obj by way of rigid bodies, not by collision objects
    void AddShape(Object* obj,float x = 0, float y = 0, float z = 0,
		  int bodyType = 0);

    void AddGhost(float x = 0, float y = 0, float z = 0);
    void MoveGhost(btTransform trans);

    void resetRotation(Object* obj);

    void moveObject(std::vector<Object*>& objs, int objIndex,
		    float x, float y, float z);

    void OutputCollisionObjects() const;

    btCollisionObject* getCollisionObject(unsigned int i);
    int getNumCollisionObjects() { return dynamicsWorld->getNumCollisionObjects(); }

    // remove by given object
    void removeCollisionObject(Object* toDelete);

    // Basically remove every object in the dynamics world array for a clean clear
    void clearDynamicsWorld();
    
    btDiscreteDynamicsWorld *dynamicsWorld;

    Sound basketHit;

  private:   
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btGhostObject* ghostObj;
};

#endif  /* PHYSICS_H */
