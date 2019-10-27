#include "physics.h"

Physics::Physics()
{
  broadphase = nullptr;
  collisionConfiguration = nullptr;
  dispatcher = nullptr;
  solver = nullptr;
  dynamicsWorld = nullptr;
}

Physics::~Physics()
{
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

  dynamicsWorld->setGravity(btVector3(0, -9.81, 0)); //sets gravity

  return true;
}

// Step through dynamics world simulation
void Physics::Update() {
  dynamicsWorld->stepSimulation(1.0f/60.f, 10);

  // Display rigidbody info for testing purposes
  std::cout << "num Collision Objects: " << dynamicsWorld->getNumCollisionObjects() << std::endl;
  btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[0];
  btRigidBody* body = btRigidBody::upcast(obj);
  btTransform trans;

  if(body && body->getMotionState()) {
    body->getMotionState()->getWorldTransform(trans);
  }
  else {
    trans = obj->getWorldTransform();
  }

  std::cout << "World position of object: " << float(trans.getOrigin().getX()) << " "
	    << trans.getOrigin().getY() << " "
	    << trans.getOrigin().getZ() << std::endl;
}

/* Add btCollisionObject given by newly initialized object to physics->dynamicsWorld;

 */
void Physics::AddShape(Object* obj)
{
  std::cout << "Entered add shape function in physics.cpp" << std::endl;

  btDefaultMotionState *shapeMotionState = \
    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 0),
					 btVector3(0, 0, 0)));
  std::cout << "Created motion state" << std::endl;
  
  btScalar mass(1);
  btVector3 inertia(0, 0, 0);
  std::cout << "Created inertia and mass " << std::endl;
  obj->shape->calculateLocalInertia(mass, inertia);
  //btCollisionShape* test = new btBoxShape(btVector3(1, 1, 1));
  //test->calculateLocalInertia(mass, inertia);
  std::cout << "Calculated local inertia" << std::endl;

  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState,
							    obj->shape, inertia);
  btRigidBody *rigidBody = new btRigidBody(shapeRigidBodyCI);

  int COLLIDE_MASK = 4;
  bool CollidesWith = true;
  dynamicsWorld->addRigidBody(rigidBody, COLLIDE_MASK, CollidesWith);
  std::cout << "Added shape" << std::endl;
}

