#include "engine.h"
#include "camera.h"
#include "stdlib.h"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}


void Engine::createObject(const std::string& objFileName, const ShapeInfo& newShape,
			  const std::string& key, const std::string& texFileName,
			  const float& x, const float& y, const float& z, const float& Rtype){

  Object* temp = new Object(objFileName, newShape, key, texFileName);
  objs.push_back(temp);
  m_physics->AddShape(temp,
		      x, y, z,
		      Rtype);
}
void Engine::createDisk(const std::string& objFileName, const ShapeInfo& newShape,
			  const std::string& key, const std::string& texFileName,
			  const float& x, const float& y, const float& z, const float& Rtype){

  Object* temp = new Object(objFileName, newShape, key, texFileName);
  disks.push_back(temp);
  m_physics->AddShape(temp,
		      x, y, z,
		      Rtype);
}

bool Engine::Initialize(char **argv)
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("window failed to initialize.\n");
    return false;
  }  

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, argv)) //initialize new instance of Objects here
  {
    printf("The graphics failed to initialize.\n");
    return false;
  } 

 // Start the physics
   m_physics = new Physics(this);
   if(!m_physics->Initialize()) //set the 5 components of a physics world here
   {
     printf("The physics failed to initialize.\n");
     return false;
   }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  // ============= Initialize game logic ===============================
  playing = false;
  levelLoaded = false;
  score = 0;
  timer = MAX_TIME;

  maxDisks = 1;

  // ============= Create the objects ===============================
  // note : Objects in obj array have graphics and physics components
  //        automatically updated and rendered
  //        in Engine::Run()
  // Format is as follows: 
  // create ShapeInfo struct > ShapeInfo(Shape s, float x=0, float y=0, float z=0) - x, y, z are size, unneeded for mesh
  // use struct to initialize object > Object(const std::string& objFileName, const ShapeInfo& newShape,
  //                                        	const std::string& key = NA, const std::string& texFileName = NA)
  // push object into objs array
  // add object to physics world > void AddShape(Object* obj,float x = 0, float y = 0, float z = 0, int bodyType = 0)
  //                                x, y, z are initial coordinates, bodyType: 1 = dynamic, 2 = kinematic, 3 = static
  //                                note: mesh cannot be dynamic


  // Add basket : Kinematic (type 2)
  struct ShapeInfo bucketInfo(mesh);
  createObject("bucket.obj", bucketInfo, "bucket", "steel.jpg", 0, -16, -1.25, 2);
  basketIndex = objs.size() - 1;

  m_physics->AddGhost(0, -16, -1.25);
  
  // add invisible wall :: i0
  struct ShapeInfo invWallInfo(box, 10, 15, 0.3);
  //createObject("bucket.obj", invWallInfo, "glassTop", NA, 0, 0, -5, 3);
  createObject("verticalboard.obj", invWallInfo, "glassTop", NA, 0, 0, -1, 3);
  //createObject("verticalboard.obj", invWallInfo, "regTop", "steel.jpg", 0,0,-3,3); // Visible version for testing
  m_physics->getCollisionObject(m_physics->getNumCollisionObjects()-1)->setFriction(btScalar(0.0f));

  // Add board : Static (type 3)
  struct ShapeInfo boardInfo(box, 10,15, 0.5);
  createObject("verticalboard.obj", boardInfo, "board", "wood.jpg", 0, 0, 0, 3);
  m_physics->getCollisionObject(m_physics->getNumCollisionObjects()-1)->setFriction(btScalar(0.0f));

  // Try to add ghost object
  // struct ShapeInfo ghostTest(ghostObject_mesh);
  // createObject("ghost.obj", ghostTest, "ghost", "galaxy.jpg", 0, -5, -3, 4);
  // ghostIndex = objs.size() -1;
 
  // Add Pegs : Static (type 3)
  // TODO: instantiate pegs to cut down on rendering
  struct ShapeInfo pegInfo(cylind, 0.2, 3, 3);
  //struct ShapeInfo pegInfo(mesh);
  int z = 0;
  for(int y = -9; y <= 9; y += 3){ // rows at -9, -6, 3, 0, 3, 6, 9
    if(y == -9 || y == -3 || y == 3 || y == 9){
      for(int x = -3; x <= 6; x += 3){ // columns at -3, 0, 3, 6,
        createObject("peg.obj", pegInfo, "peg", "metal.jpg", x, y, z, 3);
	m_physics->getCollisionObject(m_physics->getNumCollisionObjects()-1)->setFriction(btScalar(0.0f));
      }
    }
    else{
     for(int x = -4.5; x <= 7.5; x += 3){ // columns at -4.5, -1.5, 1.5, 4.5, 7.5
        createObject("peg.obj", pegInfo, "peg", "metal.jpg", x, y, z, 3);	
	m_physics->getCollisionObject(m_physics->getNumCollisionObjects()-1)->setFriction(btScalar(0.0f));
      }
    }
  }
  
  // Add Triangular Walls // see above for instancing problem
  struct ShapeInfo triangleInfo(mesh);
  // Create left wall
  {
    for(int y = 13; y >= -12; y -= 4) {
      for(int x = -8; x <= 10; x += 18){
         if(x == -8){
           createObject("triangleprism.obj", triangleInfo, "leftwall", "wood.jpg", x, y, 0, 3);
         }
         else{
           createObject("trianglprismL.obj", triangleInfo, "rightwall", "wood.jpg", x, y, 0, 3);
         }
       }
    }
  }

  // Add disks : Dynamic (type 1)
  // struct ShapeInfo diskInfo(cylind, 0.75,  0.75,  0.75);
  // //createObject("disk.obj", diskInfo, "disk", "galaxy.jpg", 0, 10, -3, 1);
  // createDisk("disk.obj", diskInfo, "disk", "galaxy.jpg", 0, 10, -0.5, 1);

  levelLoaded = true;

  //  std::cout << "Object Initial values\n";
  //  outputObjects();

  // Level 0 initialization moved to function, uncomment when ready to move to function
  // body of code above should be copied to loadLevel when finalized
  //loadLevel(0);


//  for(int i = 0; i < objs.size(); i++)
//  {
//    objs[i]->physicsObject->setUserPointer(objs[i]);
//  }
//   for(int i = 0; i < objs.size(); i++)
//  {
//    std::cout << ((Object*)(objs[i]->physicsObject->getUserPointer()))->getKeyname() << std::endl;
//  }

  //outputObjects();
  
  
//  ========================= End Object Creation :> =================


  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
    }

    // if there's no level, aka no objects to update then don't run
    if(!levelLoaded) {
      m_graphics->clearScreen();
      //std::cout << "No level loaded." << std::endl;
      continue;
    }

    // Update physics
    //m_physics->Update();
    m_physics->Update(objs, disks);
    //outputObjects();

    // DEBUG COLLISION TESTING for danny phantom
    /*
    std::cout << "Testing ghost collision?\n";
    btGhostObject* tempGhost = nullptr;
    tempGhost = btGhostObject::upcast(objs[ghostIndex]->physicsObject);
    if(tempGhost != nullptr) {
      // Test for ghost object collision here and output to terminal
      std::cout << tempGhost->getNumOverlappingObjects()
	      << std::endl;
    }
    //else
      //std::cout << "Tempghost is nullptr" << std::endl;
      */
    

    // Update Graphics, send in physics instance and each single object.
    for(int i = 0; i < objs.size(); i++) {
      m_graphics->Update(objs[i]);
    }
    for(int i = 0; i < disks.size(); i++) {
      m_graphics->Update(disks[i]);
    }

    // Render, send in ALL OBJECTS stored in composite objects
    std::vector<Object*> compositeObjects = objs;
    // add the rest of the objects to the composite objects array
    for(int i = 0 ;  i < disks.size(); i++) {
      compositeObjects.push_back(disks[i]);
    }
    m_graphics->Render(compositeObjects);
    //std::cout << "Graphics successfully rendered\n";

    // Game Logic
    deleteOutOfBoundsDisks();
     
    // Swap to the Window
    m_window->Swap();

    //std::cout <<timer << std::endl;
    if(playing){
      //cout << timer << std::endl;
      timer -= getDT();
      if(timer <= 40000 && spawn40){
        //std::cout << "40 secs" << std::endl;
        spawnDisk();
        spawn40 = false;
      }
      else if(timer <= 20000 && spawn20){
        //std::cout << "20 secs" << std::endl;
        spawnDisk();
        spawn20 = false;
      }
      else if(timer <= 0){
        std::cout << "GAME OVER" << std::endl;
        sound.loadSound(GAMEOVER_SOUND);
        sound.launchSound();
        playing = false;
      }
    }
  }
  return;
}

/*
  This Function parses file information in order to load a level

 void Engine::loadLevel(std::string levelFile) {

 }
*/

void Engine::increaseScore(int amount)
{
  score += amount;
  std::cout << "Score: " << score << std::endl;
}

void Engine::Keyboard()
{
  btTransform newTrans;
  int randSpawnVal = 0;
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if (m_event.type == SDL_KEYDOWN)
  {
    // handle key down events here
    switch(m_event.key.keysym.sym)
    {
      case SDLK_ESCAPE:
      m_running = false; //quit the program
      break;

      /**************************SHADER CONTROLS**************************/
      //toggle to vert shader
      case SDLK_v:
        m_graphics->toggleShader(0);
        break;

      //toggle to frag shader
      case SDLK_f:
        m_graphics->toggleShader(1);
        break;

      // toggle flat shading
      case SDLK_c:
        m_graphics->toggleShader(2);
        break;

      /**************************LIGHTING CONTROLS**************************/
      case SDLK_u: // increase ambient lighting
        //std::cout << "Ambience: " << m_graphics->ambience.x << " " <<  m_graphics->ambience.y << " " <<  m_graphics->ambience.z << std::endl;
        m_graphics->ambience.x += 0.05f;
        m_graphics->ambience.y += 0.05f;
        m_graphics->ambience.z += 0.05f;
        break;

      case SDLK_y: //decrease ambient lighting
        //std::cout <<  "Ambience: " << m_graphics->ambience.x << " " <<  m_graphics->ambience.y << " " <<  m_graphics->ambience.z << std::endl;
        m_graphics->ambience.x -= 0.05f;
        m_graphics->ambience.y -= 0.05f;
        m_graphics->ambience.z -= 0.05f;
        break;

      case SDLK_h: //increase cubes diffuse lighting
        //std::cout << "Cube Diffuse: " <<  m_graphics->diffuse.x << " " <<  m_graphics->diffuse.y << " " <<  m_graphics->diffuse.z << std::endl;
        m_graphics->diffuse.x += 0.05f;
        m_graphics->diffuse.y += 0.05f;
        m_graphics->diffuse.z += 0.05f;
        break;

      case SDLK_j: //decrease cubes diffuse lighting
        //std::cout << "Cube Diffuse: " <<  m_graphics->diffuse.x << " " <<  m_graphics->diffuse.y << " " <<  m_graphics->diffuse.z << std::endl;
        m_graphics->diffuse.x -= 0.05f;
        m_graphics->diffuse.y -= 0.05f;
        m_graphics->diffuse.z -= 0.05f;
        break;

      case SDLK_n: //increase cubes specular lighting
        //std::cout << "Cube Specular: " <<  m_graphics->specular.x << " " <<  m_graphics->specular.y << " " <<  m_graphics->specular.z << std::endl;
        m_graphics->specular.x += 0.05f;
        m_graphics->specular.y += 0.05f;
        m_graphics->specular.z += 0.05f;
        break;

      case SDLK_m: //decrease cubes specular lighting
        //std::cout << "Cube Specular: " <<  m_graphics->specular.x << " " <<  m_graphics->specular.y << " " <<  m_graphics->specular.z << std::endl;
        m_graphics->specular.x -= 0.05f;
        m_graphics->specular.y -= 0.05f;
        m_graphics->specular.z -= 0.05f;
        break;

	/**************************GAME CONTROLS**************************/
    case SDLK_d: // Set to default view
      m_graphics->m_camera->Update(0,0,-35,
				   0,0,0,
				   0,1,0);
      break;
    case SDLK_t: // Set to top down camera view
      m_graphics->m_camera->Update(0,30,0,
				   0,0,0,
				   0,0,1);
      break;
    case SDLK_LEFT:
      objs[basketIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
		  
      objs[basketIndex]->RBody->getMotionState()->getWorldTransform(newTrans);
      newTrans.getOrigin() += btVector3(1, 0, 0);
      if(newTrans.getOrigin().getX() < 10)
      {
        objs[basketIndex]->RBody->getMotionState()->setWorldTransform(newTrans);
        m_physics->MoveGhost(newTrans); 
      }
      break;
    case SDLK_RIGHT:
      objs[basketIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
		  
      objs[basketIndex]->RBody->getMotionState()->getWorldTransform(newTrans);
      newTrans.getOrigin() += btVector3(-1, 0, 0);
      if(newTrans.getOrigin().getX() > -8)
      {
        objs[basketIndex]->RBody->getMotionState()->setWorldTransform(newTrans);
        m_physics->MoveGhost(newTrans); 
      }
      break;

    case SDLK_r: //respawn each disk


      break;
      
    case SDLK_l: // Add disk
    /*
      {
      struct ShapeInfo defaultDisk(cylind, 0.75, 0.75, 0.75);
      createDisk("disk.obj", defaultDisk, "disk", "galaxy.jpg", 0,0,0,1);

      // spawn in random position
      randSpawnVal = rand() % 16 + (-6); //generate a random number from -6 to 6
      m_physics->resetRotation(disks.back());
      m_physics->moveObject(disks, disks.size()-1,
			    randSpawnVal, 13, -0.5);
    }
    */
      spawnDisk();
      break;
      
    case SDLK_k: // Remove disk
      deleteObject(disks, disks.size()-1);
      break;

          // DEBUGGING , not for GAME USE
    case SDLK_b: // Show Collision Objects list and show objects in objects
      m_physics->OutputCollisionObjects();

      std::cout << "Engine Objects: " << std::endl;
      outputObjects();
      break;

    case SDLK_x: // Clear board
      clearObjects();
      std::cout << "Objects cleared!" << std::endl;
      break;

    case SDLK_SPACE: //start the game!
      timer = MAX_TIME;
      playing = true;
      score = 0;
      spawnDisk();
      break;

    case SDLK_1: // Load Level 1
      loadLevel(1);

      break;
    default:
      break;
    }
  }
  // else if (m_event.type == SDL_KEYUP)
  // {
  //   // handle key up events here
  //   switch(m_event.key.keysym.sym)
  //   {
		/**************************GAME CONTROLS**************************/
		//case SDLK_LEFT:
		//	objs[basketIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
		//	objs[basketIndex]->RBody->setLinearVelocity(btVector3(0, 0, 0));
		//	break;
		//case SDLK_RIGHT:
		//	objs[basketIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
		//	objs[basketIndex]->RBody->setLinearVelocity(btVector3(0, 0, 0));
		//	break;

  //     default:
  //       break;
  //   }
  // }
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}

/* 
   Outputs indexes and keynames of all objects in objs
 */
void Engine::outputObjects() const {
  m_physics->OutputCollisionObjects();
  
  std::cout << "== Objects in engine: " << objs.size() << " + " << disks.size() << std::endl;
  for(int i = 0; i < objs.size(); i++) {
    std::cout << i << ": " << objs[i]->getKeyname()
	      << " x" << objs[i]->x
	      << " y" << objs[i]->y
	      << " z" << objs[i]->z
	      << std::endl;
  }
  std::cout << std::endl;
  for(int i = 0; i < disks.size(); i++) {
    std::cout << i << ": " << disks[i]->getKeyname()
      	      << " x" << disks[i]->x
	      << " y" << disks[i]->y
	      << " z" << disks[i]->z
	      << std::endl;
  }
  std::cout << "=======================" << std::endl;
}

int Engine::getIndexOf(const std::string& key) {
  for(int i = 0; i < objs.size(); i++) {
    if (objs[i]->getKeyname() == key)
      return i;
  }
  return -1; // Key not found 
}

void Engine::deleteOutOfBoundsDisks() {
  int boundary = -15;

  for(int i = 0; i < disks.size(); i++) {
    if(disks[i]->y <= boundary) {
      deleteObject(disks, i);
    }
  }
}

/*
  Use this to remove an object from one of the arrays without the issue of leaks(particularly 
  when considering m_physics->dynamicsWorld)
 */
void Engine::deleteObject(std::vector<Object*>& objArray, int objIndex) {
  
  // Remove from physics context
  m_physics->removeCollisionObject(objArray.at(objIndex));

  
  // Remove from Engine object array
  std::vector<Object*>::iterator i = objArray.begin() + objIndex;
  objArray.erase(i);


  return;
}

// void Engine::deleteDisk(Object* disk) {
  
//   // Remove from physics context
//   m_physics->removeCollisionObject(disk->physicsObject);
  
//   // Remove from Engine disks array
//   std::vector<Object*>::iterator i = disks.begin() + objIndex;
//   objArray.erase(i);

//   return;
// }

/*
  Clears most objects and disks
 */
void Engine::clearObjects() {
  int i;

  // Set indexes to -1
  basketIndex = -1;

  // No Level is loaded...
  levelLoaded = false;

  // clear all objs
  for(i = objs.size()-1; i >= 0; i--) {
    deleteObject(objs, i);
  }
  
  // clear disks
  for(i = disks.size()-1; i >= 0; i--) {
    deleteObject(disks, i);
  }

  // CLear any remaining objects from dynamics world
  m_physics->clearDynamicsWorld();

  outputObjects();

  // Clear Screen
  m_graphics->clearScreen();

  return;
}

/*
  This function loads levels corresponding to a string file or defaults to level 0, which is what the
  game starts with.
 */
void Engine::loadLevel(int level) {

  clearObjects(); // Clears other objects
  
  std::cout << "Loading level " << level << std::endl;
  
  // ========== Load Levels down below ==========
  if(level == 0) { // Default level

    // Insert object initialization here for default level

    // Add basket : Kinematic (type 2)
    struct ShapeInfo bucketInfo(mesh);
    createObject("bucket.obj", bucketInfo, "bucket", "steel.jpg", 0, -16, -1.25, 2);
    basketIndex = objs.size() - 1;

    m_physics->AddGhost(0, -16, -1.25);

    levelLoaded = true;

  } // End load Level 0

  // Add basket because it's common amongst All Levels
  // Add basket : Kinematic (type 2)
  struct ShapeInfo bucketInfo(mesh);
  createObject("bucket.obj", bucketInfo, "bucket", "steel.jpg", 0, -16, -1.25, 2);
  basketIndex = objs.size() - 1;

  m_physics->AddGhost(0, -16, -1.25);

  switch(level) {
    //std::fstream lvlFile("../Assets/level1.txt");
  }

  // Display all the information for testing purposes before attempting to load


  std::cout << "Level " << level << " loaded.\n";
}

void Engine::spawnDisk() {
  //spawn disk
  struct ShapeInfo defaultDisk(cylind, 0.75, 0.1, 0.1);
  createDisk("disk.obj", defaultDisk, "disk", "galaxy.jpg", 0,0,0,1);

  // spawn in random position
  srand(time(0));
  float  randSpawnVal = (rand() % 16) + (-6); //generate a random number from -6 to 9?
  std::cout << "Random spawn val: " << randSpawnVal << std::endl;
  m_physics->resetRotation(disks.back());
  m_physics->moveObject(disks, disks.size()-1,
			randSpawnVal, 12, -0.5);
  // Spawn with minor downward force
  //disks.back()->RBody->setLinearVelocity(btVector3(0,-10,0));
}
