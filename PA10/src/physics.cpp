#include "physics.h"

Physics::Physics()
{
	lostBall = false;
	ballIndex = 0;
	zCoordTrigger = -5.5f;
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

  dynamicsWorld->setGravity(btVector3(0, -9.81, -1)); //sets gravity, last value sets gravit at an angle

  // Set ballIndex

  return true;
}

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

  // Check if the ball has been lost
  if (trans.getOrigin().getZ() <= zCoordTrigger)
	  lostBall = true;
}

void Physics::Update(std::vector<Object*>& objs) {
  dynamicsWorld->stepSimulation(1.0f/20.f, 10); //sped up simulation speed

  
  btTransform trans; // Stores transformations
  btScalar m[16]; // 4x4 matrix to store transformations
  // Update the position of every object
  for(int i = 0; i < objs.size(); i++) {
    objs[i]->RBody->getMotionState()->getWorldTransform(trans);
    trans.getOpenGLMatrix(m);
    objs[i]->setPosition( (float)m[12], (float)m[13], (float)m[14] ); // store updated position

    // Find pinball too
    if(objs[i]->getKeyname() == "pinball") {
      setBallIndex(i);
    }
  }
    
  // Check if the ball has been lost  
  if (objs[ballIndex]->z <= zCoordTrigger) {
	  lostBall = true;
  }
}

void Physics::moveObject(std::vector<Object*>& objs, int objIndex,
			 float x, float y, float z) {
  // Check if objIndex is valid first
  if(objIndex >= 0 &&
     objIndex < objs.size()) {}
  else {
    std::cout << "Index not valid\n";
    return;
  }

  /*
  btVector3 newPos;
  newPos.setX( btScalar(x) );
  newPos.setY( btScalar(y) );
  newPos.setZ( btScalar(z) );
  */

  btTransform trans; // to store transform
  btScalar m[16];
  
  objs[objIndex]->RBody->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);

  // Change transformation matrix values
  m[12] = x; m[13] = y; m[14] = z;

  // Re-set the transform
  trans.setFromOpenGLMatrix(m);
  objs[objIndex]->RBody->getMotionState()->setWorldTransform(trans);
  objs[objIndex]->RBody->setWorldTransform(trans);
  objs[objIndex]->RBody->setLinearVelocity( btVector3(0,0,0) );
  objs[objIndex]->RBody->setAngularVelocity( btVector3(0,0,0) );

  // Store updated position
  objs[objIndex]->setPosition( x,y,z );
  

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

void Physics::movePaddle(unsigned int dt, std::string LeftOrRight,  btRigidBody *RBody){
  
    btTransform turn;
    turn.setIdentity();
    btQuaternion quat;  
    btScalar x, y, z;
  
  if(LeftOrRight == "left"){ //update left paddle with physics
    std::cout << "updating left paddle" << std::endl;

    /*
    RBody->getWorldTransform().getBasis().getEulerZYX(z, y, x);
    RBody->getMotionState()->getWorldTransform( turn );
       y += dt * M_PI/250;
        // std::cout << "This is after: " << y << std::endl;
        if( y > 1.55 )
      {
        y = 1.5;     
      }
      quat.setEulerZYX( 0, y , 0 );
      turn.setRotation(quat);
      RBody->getMotionState( )->setWorldTransform( turn );
    */

  }
  
  else if(LeftOrRight == "right"){ //update right paddle with physics
    std::cout << "updating right paddle" << std::endl;
    /*
    RBody->getWorldTransform().getBasis().getEulerZYX(z, y, x);
    RBody->getMotionState()->getWorldTransform( turn );
      y -= dt * M_1_PI/250; //controls the rotation size increment
      if( y < -1.55 ){ //if past 90 degrees, set to 90 degrees
        y = -1.5;     
      }
      quat.setEulerZYX( 0, y , 0 );
      turn.setRotation(quat);
      RBody->getMotionState( )->setWorldTransform( turn );
    */
    
  }

  else{
    std::cout << "Paddle " << LeftOrRight << "not Specified" << std::endl;
  }

}
