#include "physics.h"

Physics::Physics()
{

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

  dynamicsWorld->setGravity(btVector3(0, -9.81, -0.8)); //sets gravity, last value sets gravit at an angle

  return true;
}

// Step through dynamics world simulation and output for debugging purposes
void Physics::Update() {
  dynamicsWorld->stepSimulation(1.0f/20.f, 10); //sped up simulation speed
  //OutputCollisionObjects();

    //check to see if ball has collided with the box to trigger losing the ball
  //code modified from https://www.raywenderlich.com/2606-bullet-physics-tutorial-getting-started
  // int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
  // for(int i = 0; i < numManifolds; i++)
  // {
  //   btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
  //   const btCollisionObject* obA = contactManifold->getBody0();
  //   const btCollisionObject* obB = contactManifold->getBody1();

  //   if (contactManifold->getNumContacts() > 0)
  //   {
  //     if(obA == objs[ballIndex]->physicsObject && obB == objs[trigIndex]->physicsObject && !lostBall)
  //       lostBall = true;
  //     else if(obB == objs[ballIndex]->physicsObject && obA == objs[trigIndex]->physicsObject && !lostBall)
  //       lostBall = true;
  //     else
  //       lostBall = false;
  //   }
  // }
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
void Physics::AddShape(Object* obj, float x, float y, float z, bool dynamic)
{
  // btVector3 stores the initial starting position in xyz
  btDefaultMotionState *shapeMotionState = \
    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
					 btVector3(x, y, z)));
 
  btScalar mass = (dynamic) ? 1 : 0;
  
  btVector3 inertia(0, 0, 0);

  obj->shape->calculateLocalInertia(mass, inertia);
  
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState,
							    obj->shape, inertia);
  
  btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);
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
