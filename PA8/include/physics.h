#ifndef PHYSICS_H
#define PHYSICS_H

#include "graphics_headers.h"
#include "object.h"

class Physics
{
  public:
    Physics();
    ~Physics();
    bool Initialize();

    void Update();

    void AddShape(Object* obj);

  private:

    // This vector is parallel to the one in graphics?
    // std::vector<Object*> objs
    
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;
};

#endif  /* PHYSICS_H */
