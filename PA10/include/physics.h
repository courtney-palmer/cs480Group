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
    void Update(std::vector<Object*>& objs); 
    //void Update(std::vector<Object*>& objs, unsigned int ballIndex, unsigned int trigIndex);

    // default : obj starts at origin as dynamic collision object
    void AddShape(Object* obj,float x = 0, float y = 0, float z = 0, int bodyType = 0);

    void moveObject(std::vector<Object*>& objs, int objIndex,
		    float x, float y, float z);

    void OutputCollisionObjects() const;

    void movePaddle(unsigned int dt, std::string LeftOrRight,  btRigidBody *RBody);
    void updatePaddle(btRigidBody* body, bool right); // Returns paddle to original position
    btTransform ogLPaddleTrans, ogRPaddleTrans;
    void resetPaddle(std::string LeftOrRight,  btRigidBody *RBody);
    
    btDiscreteDynamicsWorld *dynamicsWorld;
    float rightBumperAngle, leftBumperAngle;
    void setBallIndex(int i) {ballIndex = i;}
    int getBallIndex() const {return ballIndex;}

    //void applyPlungerForce(btVector3 vel, btRigidBody *RBody, float ballForce);
    void applyPlungerForce(btRigidBody *RBody);


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

/*
 
    rightBumperAngle -= 0.2f;
    if(rightBumperAngle < -1.5f){
      rightBumperAngle = -1.5f;
    }
    leftBumperAngle += 0.2f;
    std::cout << leftBumperAngle << std::endl;
    if(leftBumperAngle > 1.5f){
      leftBumperAngle = 1.5f;
    }
    btTransform turn;
    turn.setIdentity();
    btQuaternion quat;  
    btScalar x, y, z;
  
  if(LeftOrRight == "left"){ //update left paddle with physics
    RBody->getMotionState()->getWorldTransform(turn);    
    quat.setEuler(leftBumperAngle, 0.0, 0.0);
    turn.setRotation(quat);
    RBody->getMotionState()->setWorldTransform(turn);
    RBody->setMotionState(RBody->getMotionState());

  }
  
  else if(LeftOrRight == "right"){ //update right paddle with physics
    RBody->getMotionState()->getWorldTransform(turn);    
    quat.setEuler(rightBumperAngle, 0.0, 0.0);
    turn.setRotation(quat);
    RBody->getMotionState()->setWorldTransform(turn);
    RBody->setMotionState(RBody->getMotionState());
  }
  */