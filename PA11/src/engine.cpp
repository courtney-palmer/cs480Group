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

bool Engine::Initialize(char **argv)
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("window failed to initialize.\n");
    return false;
  }  

  // objectCollidedSound.loadSound(HIT_SOUND);
  // objectCollidedSound.launchSound();
  // objectCollidedSound.playSound();

  //start sound
  // m_sound = new Sound();
  // m_sound->Initialize();

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
  score = 0;
  timer = MAX_TIME;

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


  // add invisible wall
  struct ShapeInfo invWallInfo(box, 15, 15, 1);
  createObject("bucket.obj", invWallInfo, "glassTop", NA, 0, 0, -5, 3);

  // Add board : Static (type 3)
  struct ShapeInfo boardInfo(mesh);
  createObject("window.obj", boardInfo, "board", "wood.jpg", 0, 0, 0, 3);

  // Add disks : Dynamic (type 1)
  struct ShapeInfo diskInfo(cylind, 0.75,  0.75,  0.75);
  createObject("disk.obj", diskInfo, "disk", "galaxy.jpg", 0, 10, -3, 1);
  diskIndex = objs.size()-1;

  // Add basket : Kinematic (type 2)
  struct ShapeInfo bucketInfo(mesh);
  createObject("bucket.obj", bucketInfo, "bucket", "steel.jpg", 0, -7, -3, 2);
  basketIndex = objs.size() - 1;
 
  // Add Pegs : Static (type 3)
  // TODO: instantiate pegs to cut down on rendering
  struct ShapeInfo pegInfo(mesh);
  // for(int y = -3; y <= 6; y += 3){ // rows at -3, 0, 3, 6
  //   for(int x = -9; x <= 9; x += 3){ // columns at -9, -6, -3, 0, 3, 6, 9
  //     if(y == 0 || y == 6) // add an extra offset for alternating rows
  //       x += 1.5;
      createObject("peg.obj", pegInfo, "peg", "metal.jpg", 0, 0, 0, 3);
  //   }
  // }

  /* WIP
  // Try to add ghost object
  struct ShapeInfo ghostTest(ghostObject_mesh);
  temp = new Object("bucket.obj", ghostTest, "ghost", "wood.jpg");
  objs.push_back(temp);
  m_physics->AddShape(temp, 0,-5,-3, 3);
  */
  
//  ========================= End Object Creation :> =================

  buffer = 0;
  bufferMax = 1;

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

    // Update physics
    m_physics->Update(objs, score);

    // Update Graphics, send in physics instance and each single object.
    for(int i = 0; i < objs.size(); i++) {
      m_graphics->Update(objs[i]);
    }
    // Render, send in objs vector array
    m_graphics->Render(objs);
     
    // Swap to the Window
    m_window->Swap();
  }
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
	  case SDLK_LEFT:
		  objs[basketIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
		  
		  objs[basketIndex]->RBody->getMotionState()->getWorldTransform(newTrans);
		  newTrans.getOrigin() += btVector3(1, 0, 0);
		  objs[basketIndex]->RBody->getMotionState()->setWorldTransform(newTrans);
		  //objs[basketIndex]->RBody->setLinearVelocity(btVector3(10, 0, 0));
		  break;
	  case SDLK_RIGHT:
		  objs[basketIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
		  
		  objs[basketIndex]->RBody->getMotionState()->getWorldTransform(newTrans);
		  newTrans.getOrigin() += btVector3(-1, 0, 0);
		  objs[basketIndex]->RBody->getMotionState()->setWorldTransform(newTrans);
		  //objs[basketIndex]->RBody->setLinearVelocity(btVector3(-10, 0, 0));
		  break;

    case SDLK_r: //restart disk
      randSpawnVal = rand() % 16 + (-6); //generate a random number from -6 to 6
      m_physics->resetRotation(objs[diskIndex]);
      m_physics->moveObject(objs, diskIndex,
			    randSpawnVal, 6, -3);
      
      default:
        break;
    }
  }
  else if (m_event.type == SDL_KEYUP)
  {
    // handle key up events here
    switch(m_event.key.keysym.sym)
    {
		/**************************GAME CONTROLS**************************/
		//case SDLK_LEFT:
		//	objs[basketIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
		//	objs[basketIndex]->RBody->setLinearVelocity(btVector3(0, 0, 0));
		//	break;
		//case SDLK_RIGHT:
		//	objs[basketIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
		//	objs[basketIndex]->RBody->setLinearVelocity(btVector3(0, 0, 0));
		//	break;

      default:
        break;
    }
  }
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
  std::cout << "== Objects in engine ==" << std::endl;
  for(int i = 0; i < objs.size(); i++) {
    std::cout << i << ": " << objs[i]->getKeyname()
	      << " x" << objs[i]->x
	      << " y" << objs[i]->y
	      << " z" << objs[i]->z
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
