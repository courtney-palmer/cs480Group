#ifndef PHYSICS_H
#define PHYSICS_H

#include "graphics_headers.h"
#include "object.h"

#include <cmath>

class Engine;

class Physics
{
  public:
    Physics(Engine* e);
    ~Physics();
    bool Initialize();

    void Update(std::vector<Object*>& objs, std::vector<Object*>& disks); // just used to stepSimulation and update positions
    void Update(std::vector<Object*>& objs, unsigned int& score);
    
    // default : obj starts at origin as dynamic collision object
    // inserts obj by way of rigid bodies, not by collision objects
    void AddShape(Object* obj,float x = 0, float y = 0, float z = 0,
		  int bodyType = 0);

    void resetRotation(Object* obj);

    void moveObject(std::vector<Object*>& objs, int objIndex,
		    float x, float y, float z);

    void OutputCollisionObjects() const;

    // remove by given object
    void removeCollisionObject(Object* toDelete);
    
    btDiscreteDynamicsWorld *dynamicsWorld;

  private:   
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    
    Engine* m_engine;
};

#endif  /* PHYSICS_H */
