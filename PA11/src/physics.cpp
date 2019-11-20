#include "physics.h"
#include <string>

Physics::Physics()
{
	lostBall = false;
	ballIndex = 0;
	zCoordTrigger = -7.5f;
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

  dynamicsWorld->setGravity(btVector3(0, -9.81, -3)); //sets gravity, last value sets gravit at an angle

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

  // Check if the ball has been lost
  if (trans.getOrigin().getZ() <= zCoordTrigger)
	  lostBall = true;
}

void Physics::Update(std::vector<Object*>& objs, unsigned int& score) {
  dynamicsWorld->stepSimulation(1.0f/20.f, 10); //sped up simulation speed

  //check for collisions with bumpers
  //code modified from https://www.raywenderlich.com/2606-bullet-physics-tutorial-getting-started#toc-anchor-010
  // int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
  // std::cout << "int manifolds" << std::endl;
  // for (int i=0;i<numManifolds;i++)
  // {
  //   std::cout << "i = " << i << std::endl;
	//   btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
  //   std::cout << "btPersistentManifold* contactManifold" << std::endl;
  //   int numContacts = contactManifold->getNumContacts();
  //   std::cout << "int numContacts" << std::endl;
  //   if (numContacts > 0)
  //   {
  //     std::cout << "numContacts > 0" << std::endl;
  //     const btCollisionObject* b0 = contactManifold->getBody0();
  //     std::cout << "getBody0" << std::endl;
  //     const btCollisionObject* b1 = contactManifold->getBody1();
  //     std::cout << "getBody1" << std::endl;

  //     //std::cout << ((Object*)(b0->getUserPointer()))->getKeyname() << std::endl;

  //     // Object* obj0 = (Object*)b0->getUserPointer();
  //     // std::cout << "getUserPointer0" << std::endl;
  //     // Object* obj1 = (Object*)b1->getUserPointer();
  //     // std::cout << "getUserPointer1" << std::endl;
      
  //     //the ball is the only object bumping into things,
  //     //so if either object in the collision is a bumper,
  //     //the score should increase
  //     std::string s0(((Object*)(b0->getUserPointer()))->getKeyname());
  //     std::string s1(((Object*)(b1->getUserPointer()))->getKeyname());
  //     std::cout << "assigned string names" << std::endl;
  //     std::cout << s0 << std::endl;
  //     std::cout << s1 << std::endl;
  //     // if (obj0.getKeyname() == "bumper" || obj1.getKeyname() == "bumper")
  //     // {
  //     //   std::cout << "one object is a bumper" << std::endl;
  //     //   score += 100;
  //     //   std::cout << "Score: " << score << std::endl;
  //     // }

  //   }
  // }

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

void Physics::applyPlungerForce(btRigidBody *RBody, float plungerPull, float plungerForce){
  btTransform trans;

  if( plungerForce < 20 && ballLaunched == false)
  { 
    //std::cout << plungerPull << std::endl;
    plungerPull -= .2;
    if(plungerPull < 3){
      plungerPull = 3;
    }

    RBody->getMotionState()->getWorldTransform(trans);
    trans.setOrigin(btVector3(0.0f, 0.0f, plungerPull));
    RBody->getMotionState()->setWorldTransform(trans);
    RBody->setMotionState(RBody->getMotionState());
    //plunger->model = glm::make_mat4(m);
    plungerForce += 1;
  }
  else if(ballLaunched == false){
    btTransform trans;
    plungerPull = 5;
    RBody->applyCentralImpulse(btVector3(0.0f, 0.0f, plungerForce));
    plungerForce = 0.0f;
    RBody->getMotionState()->setWorldTransform(trans);
    trans.setOrigin(btVector3(0.0f, 0.0f, plungerPull));
    RBody->getMotionState()->setWorldTransform(trans);
    RBody->setMotionState(RBody->getMotionState());
  }
}

void Physics::releasePlunger(btRigidBody *RBody, float plungerPull, float plungerForce){
  if(ballLaunched == false){
    btTransform trans;
    plungerPull = 5;
    RBody->applyCentralImpulse(btVector3(0.0f, 0.0f, plungerForce));
    plungerForce = 0.0f;
    RBody->getMotionState()->setWorldTransform(trans);
    trans.setOrigin(btVector3(0.0f, 0.0f, plungerPull));
    RBody->getMotionState()->setWorldTransform(trans);
    RBody->setMotionState(RBody->getMotionState());
  }
}

void Physics::movePaddle(unsigned int dt, std::string LeftOrRight,  btRigidBody *RBody){
  
    btTransform turn;
    turn.setIdentity();
    btQuaternion quat;  
    btScalar x, y, z;
  
  if(LeftOrRight == "left"){ //update left paddle with physics   
    RBody->getWorldTransform().getBasis().getEulerZYX(z, y, x);
    RBody->getMotionState()->getWorldTransform( turn );
    //y += dt * M_PI/250;
    y += 0.45;
    if( y > 1.55 )
    {
      y = 1.5;     
    }
    quat.setEulerZYX( 0, y , 0 );
    turn.setRotation(quat);
    RBody->getMotionState( )->setWorldTransform( turn );
    RBody->setWorldTransform(turn);
  }
  
  else if(LeftOrRight == "right"){ //update right paddle with physics
    RBody->getWorldTransform().getBasis().getEulerZYX(z, y, x);
    RBody->getMotionState()->getWorldTransform( turn );
      y -= 0.45; //controls the rotation size increment
      if( y < -1.55 ){ //if past 90 degrees, set to 90 degrees
        y = -1.5;     
      }
      quat.setEulerZYX( 0, y , 0 );
      turn.setRotation(quat);
      RBody->getMotionState( )->setWorldTransform( turn );
      RBody->setWorldTransform(turn);

    /*
    // Get initial transformation
    btTransform trans;
    RBody->getMotionState()->getWorldTransform(trans);
    // Apply transformation
    btQuaternion rotation(1,0,1,0);
    // Rotate
    trans.setRotation(rotation);
    // Apply to Rigid body & motion state
    RBody->setWorldTransform(trans);
    RBody->getMotionState()->setWorldTransform(trans);
    */
  }

  else{
    std::cout << "Paddle " << LeftOrRight << "not Specified" << std::endl;
  }

}

/*
  Returns paddle back to original position over time
  Called by Engine on both paddles
 */
void Physics::updatePaddle(btRigidBody* body, bool right) {
  btTransform turn;
  btQuaternion quat;  
  btScalar x, y, z;

  const int maxAngle = 0.3;

  if(right) {
    body->getWorldTransform().getBasis().getEulerZYX(z, y, x);
    body->getMotionState()->getWorldTransform( turn );
    y += 0.05; //controls the rotation size increment
    if( y >= maxAngle  ){ //if past 90 degrees, set to 90 degrees
      y = maxAngle;     
    }
    quat.setEulerZYX( 0, y , 0 );
    turn.setRotation(quat);
    body->getMotionState( )->setWorldTransform( turn );
    body->setWorldTransform(turn);
  }
  else {
    body->getWorldTransform().getBasis().getEulerZYX(z, y, x);
    body->getMotionState()->getWorldTransform( turn );
    y -= 0.05; //controls the rotation size increment
    if( y <= -maxAngle  ){ //if past 90 degrees, set to 90 degrees
      y = -maxAngle;     
    }
    quat.setEulerZYX( 0, y , 0 );
    turn.setRotation(quat);
    body->getMotionState( )->setWorldTransform( turn );
    body->setWorldTransform(turn);
  }
}