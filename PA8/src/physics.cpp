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

