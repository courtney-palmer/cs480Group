
#include "engine.h"
#include "camera.h"

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

bool Engine::Initialize(int argc, char **argv)
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("window failed to initialize.\n");
    return false;
  }
  std::cout << "Window loaded" << std::endl;
  
  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, argc, argv)) //initialize new instance of Objects here
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }
  std::cout << "Graphics loaded" << std::endl;
 
// Start the physics
   m_physics = new Physics();
   if(!m_physics->Initialize()) //set the 5 components of a physics world here
   {
     printf("The physics failed to initialize.\n");
     return false;
   }
   std::cout << "Physics loaded" << std::endl;

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();


  // ============= Create the objects ===============================
  // note : Objects in obj array have graphics and physics components
  //        automatically updated and rendered
  //        in Engine::Run()
  // Format is as follows: 
  // create ShapeInfo struct > use struct to initialize object
  // push object into objs array  > add object to physics world

  
  // add moveable cube
  struct ShapeInfo info(box, 1, 1, 1); // 1,1,1 represents size of collision shape
  Object* temp = new Object("cubeTest.obj", info);
  objs.push_back(temp);
  m_physics->AddShape(temp,
		      0,10,0, // 0,10,0 represents starting position of object
		      true);
  // Used in Keyboard() to refer to array index of object that will be moved
  dynamicCubeIndex = objs.size()-1;
  
  // add board/platform : static
  struct ShapeInfo boardInfo(mesh);
  temp = new Object("tray.obj", boardInfo);
  objs.push_back(temp);
  m_physics->AddShape(temp,
		      0, -8, 20,
		      false);

  // Add ball
  struct ShapeInfo ballInfo(sphere, 1, 1, 1);
  temp = new Object("sphere.obj", ballInfo);
  objs.push_back(temp);
  m_physics->AddShape(temp,
		     0,-5,0,
		     true);

  // Add cylinder
  struct ShapeInfo cylindInfo(cylind, 1, 1, 1);
  temp = new Object("cylinder.obj", cylindInfo);
  objs.push_back(temp);
  m_physics->AddShape(temp,
		     0,-10, 10,
		     false);

  // ========================= End Object Creation :> =================

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
    m_physics->Update();

    // Update Graphics, send in physics instance and each single object.
    for(int i = 0; i < objs.size(); i++) {
      m_graphics->Update(m_physics, objs[i]);
    }

    // Render, send in objs vector array
    m_graphics->Render(objs);

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{
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

      //////Input to move cube

      case SDLK_UP:
	// Example of original code :
	// m_graphics->cube->RBody->setActivationState(DISABLE_DEACTIVATION);
	// m_graphics->cube->RBody->setLinearVelocity(btVector3(0, 0, 10));
	objs[dynamicCubeIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
	objs[dynamicCubeIndex]->RBody->setLinearVelocity(btVector3(0, 0, 10));
      break;

      case SDLK_DOWN:
	objs[dynamicCubeIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
	objs[dynamicCubeIndex]->RBody->setLinearVelocity(btVector3(0, 0, -10));
      break;

      case SDLK_LEFT:
	objs[dynamicCubeIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
	objs[dynamicCubeIndex]->RBody->setLinearVelocity(btVector3(10, 0, 0));
      break;

      case SDLK_RIGHT:
	objs[dynamicCubeIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
	objs[dynamicCubeIndex]->RBody->setLinearVelocity(btVector3(-10, 0, 0));
      break;

      case SDLK_SPACE:
	objs[dynamicCubeIndex]->RBody->setActivationState(DISABLE_DEACTIVATION);
	objs[dynamicCubeIndex]->RBody->applyCentralImpulse(btVector3(0, 10, 0));
      break;

      default:
      break;
    }
  }
    else if (m_event.type == SDL_KEYUP)
  {
    // handle key down events here
    switch(m_event.key.keysym.sym)
    {
      //////make sure cube stops

      case SDLK_UP:
	objs[dynamicCubeIndex]->RBody->setLinearVelocity(btVector3(0, 0, 0));
      break;

      case SDLK_DOWN:
	objs[dynamicCubeIndex]->RBody->setLinearVelocity(btVector3(0, 0, 0));
      break;

      case SDLK_LEFT:
	objs[dynamicCubeIndex]->RBody->setLinearVelocity(btVector3(0, 0, 0));
      break;

      case SDLK_RIGHT:
	objs[dynamicCubeIndex]->RBody->setLinearVelocity(btVector3(0, 0, 0));
      break;

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
