#include "physics.h"
#include <string>
#include "object.h"
#include "engine.h"

Physics::Physics(Engine* e)
{
	m_engine = e;
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

  delete ghostObj;
  ghostObj = nullptr;
}

bool Physics::Initialize()
{
  broadphase = new btDbvtBroadphase(); //creates a broadphase that tells bullet how to check for collisions
  // init to allow ghost callback?
  broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
  
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
  dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

  // Set ballIndex

  return true;
}

void Physics::Update(std::vector<Object*>& objs, std::vector<Object*>& disks, unsigned int& score) {
 dynamicsWorld->stepSimulation(1.0f/30.f, 10); //sped up simulation speed

 btTransform trans; // Stores transformations
 btScalar m[16]; // 4x4 matrix to store transformations

  // Update the position of every object
  for(int i = 0; i < objs.size(); i++) {
    //std::cout << "objs i: " << i << std::endl;
    objs[i]->RBody->getMotionState()->getWorldTransform(trans);
    trans.getOpenGLMatrix(m);

    objs[i]->setPosition( (float)m[12], (float)m[13], (float)m[14] ); // store updated position for each obj in objs
  }

  // update position of every disk
  for(int i = 0; i < disks.size(); i++) {
    //std::cout << "disks i: " << i << std::endl;
    disks[i]->RBody->getMotionState()->getWorldTransform(trans);
    trans.getOpenGLMatrix(m);

    disks[i]->setPosition( (float)m[12], (float)m[13], (float)m[14] ); // store updated position for each obj in objs
  }

  //std::cout << "Ghost stuff\n";
  
  //// Handling ghost stuff ////
  int numObjectsInGhost = 0;
  numObjectsInGhost = ghostObj->getNumOverlappingObjects();
  for(int i=0; i<numObjectsInGhost;i++)
  {
    btCollisionObject* obj = ghostObj->getOverlappingObject(i);
    for(int d = 0; d < disks.size(); d++)
    {
      if(disks[d]->physicsObject == obj)
      {
        m_engine->deleteObject(disks, d);
        break;
      }
    }
  basketHit.loadSound(HIT_SOUND);
  basketHit.launchSound();
    score += 100;
    std::cout << "Score: " << score << std::endl;
  }
  

  //std::cout << "End ghost stuff\n";
}

// void Physics::Update(std::vector<Object*>& objs, unsigned int& score)
// {
//   dynamicsWorld->stepSimulation(1.0f/30.f, 10); //sped up simulation speed

//   btTransform trans; // Stores transformations
//   btScalar m[16]; // 4x4 matrix to store transformations
//   // Update the position of every object
//   for(int i = 0; i < objs.size(); i++)
//   {
//     objs[i]->RBody->getMotionState()->getWorldTransform(trans);
//     trans.getOpenGLMatrix(m);

//     objs[i]->setPosition( (float)m[12], (float)m[13], (float)m[14] ); // store updated position for each obj in objs
//   }

//   //check for collisions with basket
//   //code modified from https://www.raywenderlich.com/2606-bullet-physics-tutorial-getting-started#toc-anchor-010

//   // int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
//   // for (int i=0;i<numManifolds;i++)
//   // {
// 	//   btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
//   //   int numContacts = contactManifold->getNumContacts();
//   //   if (numContacts > 0)
//   //   {
//   //     // const btCollisionObject* b0 = contactManifold->getBody0();
//   //     // const btCollisionObject* b1 = contactManifold->getBody1();

//   //     //std::cout << ((Object*)(b0->getUserPointer()))->getKeyname() << std::endl;

//   //     Object* obj0 = (Object*)(contactManifold->getBody0()->getUserPointer());
//   //     if(obj0 != nullptr)
//   //       std::cout << "obj0 is " << obj0->getKeyname() << std::endl;
//   //     Object* obj1 = (Object*)(contactManifold->getBody1()->getUserPointer());
//   //     if(obj1 != nullptr)
//   //       std::cout << "obj1 is " << obj1->getKeyname() << std::endl;
     
//       // std::string s0(((Object*)(b0->getUserPointer()))->getKeyname());
//       // std::string s1(((Object*)(b1->getUserPointer()))->getKeyname());
//       // std::cout << "assigned string names" << std::endl;
//       // std::cout << s0 << std::endl;
//       // std::cout << s1 << std::endl;

//       // int obj0 = b0->getUserIndex();
//       // int obj1 = b1->getUserIndex();
//       // std::cout << obj0 << std::endl;
//       // std::cout << obj1 << std::endl;

//       // if (obj0.getKeyname() == "bumper" || obj1.getKeyname() == "bumper")
//       // {
//       //   std::cout << "one object is a bumper" << std::endl;
//       //   score += 100;
//       //   std::cout << "Score: " << score << std::endl;
//       // }
//   //  }
//   // }

//   //ghostobjects https://www.gamedev.net/forums/topic/692573-bullet-btghostobject/
//   //https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=3026

//   int numObjectsInGhost = 0;
//   numObjectsInGhost = ghostObj->getNumOverlappingObjects();
//   if(numObjectsInGhost > 0)
//     std::cout << "number of objects inside ghost: " << numObjectsInGhost << std::endl;
//   for(int i=0; i<numObjectsInGhost;++i)
//   {
//     btCollisionObject* obj = ghostObj->getOverlappingObject(i);
//     if(obj != nullptr)
//       std::cout << "obj is " << ((Object*)obj->getUserPointer())->getKeyname() << std::endl;
//     else
//       std::cout << "obj is null" << std::endl;
//   }

// }

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
  }

  obj->RBody = rigidBody;
  obj->RBody->setAngularFactor(btVector3(1.0f, 0.0f, 1.0f));
  dynamicsWorld->addRigidBody(rigidBody);
  //if(obj->physicsObject != nullptr)
    //dynamicsWorld->addCollisionObject(obj->physicsObject);
  //obj->physicsObject->setUserPointer(obj);
}

/* Add btGhostObject given by newly initialized ghost object to physics->dynamicsWorld
   aka Adds ghost object to physics environment
   @param x : float : starting x coordinate
   @param y : float : starting y coordinate
   @param z : float : starting z coordinate
   @post  dynamicsWorld adds one rigid body to its collection of collision objects

   code taken from https://www.gamedev.net/forums/topic/692573-bullet-btghostobject/
 */
void Physics::AddGhost(float x, float y, float z)
{
  btTransform world(btQuaternion(0, 0, 0, 1), btVector3(x, y, z));
  ghostObj = new btGhostObject();
  ghostObj->setCollisionShape( new btBoxShape(btVector3(1.0f, 1.0f, 1.0f)) );
  ghostObj->setWorldTransform( world );
  dynamicsWorld->addCollisionObject( ghostObj );
}

void Physics::MoveGhost(btTransform trans)
{
  ghostObj->setWorldTransform(trans);
}

void Physics::resetRotation(Object* obj) {
  btTransform trans; // to store transform
  btScalar m[16];

  obj->RBody->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);

  btScalar x[3]; // store scaling in x
  x[0] = sqrt(m[0]); // x?
  x[5] = sqrt(m[5]); // y?
  x[10] = sqrt(m[10]); // z?

  m[0] = 0; m[1] = 0; m[2] = 0;
  m[4] = 0; m[5] = 0; m[6] = 0;
  m[8] = 0; m[9] = 0; m[10] = 0;

  //trans.setRotation(btQuaternion(btVector3(0.0, 0.0, 0.0), 90)); //set an objects rotation back to 0.

  trans.setFromOpenGLMatrix(m);
  obj->RBody->getMotionState()->setWorldTransform(trans);
  obj->RBody->setWorldTransform(trans);
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
  // trans.setVelocity(); //set velocity back to 0
  // trans.setRotation(btQuaternion(btVector3(0.0, 0.0, 0.0), 90)); //set an objects rotation back to 0. 

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

/*
  Output center position of all objects registered in dynamicsWorld
  @pre  none
  @post none
 */
void Physics::OutputCollisionObjects() const {
  int upper = dynamicsWorld->getNumCollisionObjects();
  std::cout << "Number of Collision Objects: " << upper << std::endl;
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

btCollisionObject* Physics::getCollisionObject(unsigned int i) {
  // bounds checking
  if(i < 0 || i >= dynamicsWorld->getNumCollisionObjects()) {
    return nullptr;
  }

  return dynamicsWorld->getCollisionObjectArray()[i];
}

// This needs to be called before removing the object from its vector array
void Physics::removeCollisionObject(Object* toDelete) {
  if(toDelete == nullptr) {
    std::cout << "Object passed in is nullptr. Halt.\n";
    return;
  }
  
  // Remove rigid body and associated properties
  try {
    delete toDelete->RBody->getMotionState();
    delete toDelete->RBody->getCollisionShape();
    dynamicsWorld->removeRigidBody(toDelete->RBody);
  }
  catch (...) {
    std::cout << "Remove object " << toDelete->getKeyname() << " unsuccessful. Check if it has a rigid body?\n";
  }
  return;
}

// Clears ALL Objects from DynamicsWorld. Suck;
void Physics::clearDynamicsWorld() {
  int objs = dynamicsWorld->getNumCollisionObjects();
  auto objArray = dynamicsWorld->getCollisionObjectArray();
  for(int i = 0; i < objs; i++) {
    dynamicsWorld->removeCollisionObject(objArray[i]);
  }
}
