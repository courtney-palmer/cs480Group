#ifndef PHYSICS_H
#define PHYSICS_H

#include "graphics_headers.h"

class Physics
{
  public:
    Physics();
    ~Physics();
    bool Initialize();
    void AddShape(btCollisionShape *shape, Shape s);

  private:
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;
};

#endif  /* PHYSICS_H */
