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
    //void Update(std::vector<Object*>& objs, unsigned int ballIndex, unsigned int trigIndex);

    // default : obj starts at origin as dynamic collision object
    void AddShape(Object* obj,float x = 0, float y = 0, float z = 0, int bodyType = 0);

    void OutputCollisionObjects() const;

    void movePaddle(unsigned int dt, std::string LeftOrRight,  btRigidBody *RBody, bool isKeyDown);
  
    btDiscreteDynamicsWorld *dynamicsWorld;
    float rightBumperAngle = 0.6f;
    float leftBumperAngle = 0.6f;
    bool lostBall;
	int ballIndex;
	float zCoordTrigger; //If the ball hits this coordinate on the z axis, it is lost

  private:

    // This vector is parallel to the one in graphics?
    // std::vector<Object*> objs
    
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    
};

#endif  /* PHYSICS_H */
