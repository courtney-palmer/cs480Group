#ifndef PHYSICS_H
#define PHYSICS_H

#include "graphics_headers.h"
#include <btBulletDynamicsCommon.h>

class Physics
{
  public:
    Physics();
    ~Physics();
    bool Initialize();
    void Enable();
    bool AddCollisionShape();
    bool Finalize();

  private:
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;
};

#endif  /* PHYSICS_H */
