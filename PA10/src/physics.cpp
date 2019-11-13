#include "physics.h"

Physics::Physics()
{
	lostBall = false;
	ballIndex = 0;
	zCoordTrigger = -3.0f;
}

Physics::~Physics()
{
  // Delete in reverse order, starting with the collision objects in dynamics world
  for(int i = dynamicsWorld->getNumCollisionObjects()-1; i >= 0; i--) {
    // delete motion state before deleting collisionObject
    btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
    btRigidBody* body = btRigidBody::upcast(obj);
    if(body && body->getMotionState()) {
      delete body->getMotionState();
    }
    dynamicsWorld->removeCollisionObject(obj);
    delete obj;
  }

  delete dynamicsWorld;
  dynamicsWorld = nullptr;

  delete broadphase;
  broadphase = nullptr;

  delete collisionConfiguration;
  collisionConfiguration = nullptr;

  delete dispatcher;
  dispatcher = nullptr;

  delete solver;
  solver = nullptr;
}

bool Physics::Initialize()
{
  broadphase = new btDbvtBroadphase(); //creates a broadphase that tells bullet how to check for collisions
  
  collisionConfiguration = new btDefaultCollisionConfiguration(); //collision algorithm
  
  dispatcher = new btCollisionDispatcher(collisionConfiguration); //sends collision events to the objects
  
  solver = new btSequentialImpulseConstraintSolver; //makes objects interact properly with each other and the world
  
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

  if (  broadphase == nullptr ||
        collisionConfiguration == nullptr ||
        dispatcher == nullptr ||
        solver == nullptr ||
        dynamicsWorld == nullptr) 
  {
    std::cerr << "Error creating physics program\n";
    return false;
  }

  dynamicsWorld->setGravity(btVector3(0, -9.81, 0)); //sets gravity, last value sets gravit at an angle

  return true;
}

// Step through dynamics world simulation and output for debugging purposes
void Physics::Update() {
  dynamicsWorld->stepSimulation(1.0f/20.f, 10); //sped up simulation speed

  //OutputCollisionObjects();

  //check to see if ball has hit the z-coordinate that will trigger a lost ball
  btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[ballIndex];
  btRigidBody* body = btRigidBody::upcast(obj);
  btTransform trans;

  if (body && body->getMotionState()) {
	  body->getMotionState()->getWorldTransform(trans);
  }
  else {
	  trans = obj->getWorldTransform();
  }

  //std::cout << "origin is: " << trans.getOrigin().getZ() << " and z coord is: " << zCoordTrigger << std::endl;

  if (trans.getOrigin().getZ() <= zCoordTrigger)
	  lostBall = true;
}

/* Add btCollisionObject given by newly initialized object to physics->dynamicsWorld
   aka Adds object to physics environment
   @param obj : Object* : Pointer to object that will be added to dynamics world
   @param x : float : starting x coordinate
   @param y : float : starting y coordinate
   @param z : float : starting z coordinate
   @param dynamic : bool : 1 if the object is dynamic, 0 if object static/kinematic
   @post  dynamicsWorld adds one rigid body to its collection of collision objects
 */
void Physics::AddShape(Object* obj, float x, float y, float z, int bodyType)
{
  // 1 = dynamic
  // 2 = kinematic
  // 3 = static

  // btVector3 stores the initial starting position in xyz
  btDefaultMotionState *shapeMotionState = \
    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
					 btVector3(x, y, z)));
 
  btScalar mass = (bodyType == 1) ? 1 : 0;
  
  btVector3 inertia(0, 0, 0);
  obj->shape->calculateLocalInertia(mass, inertia);
  
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState,
							    obj->shape, inertia);
  
  btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);
  
  if(bodyType == 2){
    std::cout << "kinematic object" << std::endl;
    //obj->RBody->isKinematicObject();
    
    int flags = rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT;
    rigidBody->setCollisionFlags(flags);

   // obj->RBody->setCollisionFlags(obj->RBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
   // std::cout << "test" << std::endl;
  }

    obj->RBody = rigidBody;
    dynamicsWorld->addRigidBody(rigidBody);
}

/*
  Output center position of all objects registered in dynamicsWorld
  @pre  none
  @post none
 */
void Physics::OutputCollisionObjects() const {
  int upper = dynamicsWorld->getNumCollisionObjects();
  for(int i = 0; i < upper; i++) {
    btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
    btRigidBody* body = btRigidBody::upcast(obj);
    btTransform trans;

    if(body && body->getMotionState()) {
      body->getMotionState()->getWorldTransform(trans);
    }
    else {
      trans = obj->getWorldTransform();
    }

    std::cout << "World position of object " << i << ": " << float(trans.getOrigin().getX()) << " "
	      << trans.getOrigin().getY() << " "
	      << trans.getOrigin().getZ() << std::endl;
  }
  std::cout << std::endl;
}

void Physics::movePaddle(unsigned int dt, std::string LeftOrRight,  btRigidBody *RBody, bool isKeyDown){
  
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

    if(isKeyDown == false){
      quat.setEuler(0.0, 0.0, 0.0);
      turn.setRotation(quat);
      RBody->getMotionState()->setWorldTransform(turn);
      RBody->setMotionState(RBody->getMotionState());
    }   

  }
  
  else if(LeftOrRight == "right"){ //update right paddle with physics
    RBody->getMotionState()->getWorldTransform(turn);    
    quat.setEuler(rightBumperAngle, 0.0, 0.0);
    turn.setRotation(quat);
    RBody->getMotionState()->setWorldTransform(turn);
    RBody->setMotionState(RBody->getMotionState());

    if(isKeyDown == false){
      quat.setEuler(0.0, 0.0, 0.0);
      turn.setRotation(quat);
      RBody->getMotionState()->setWorldTransform(turn);
      RBody->setMotionState(RBody->getMotionState());
    }   
  }

  else{
    std::cout << "Paddle " << LeftOrRight << "not Specified" << std::endl;
  }

}