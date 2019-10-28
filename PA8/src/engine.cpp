
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

  // TESTING : load initialized graphics object into physics
  // Consider : create objects from engine and then assign graphics and physics to handle&update them?
  m_physics->AddShape(m_graphics->cube, 0, 20, 0, true); //incorporating physics with initialized object
  m_physics->AddShape(m_graphics->board, 0,-10,0, false); // board starts at origin by default
  //m_physics->AddShape(m_graphics->cylinder, 0, -30, 0, false); //incorporating physics with initialized object
  //m_physics->AddShape(m_graphics->ball, 0, 2, 0, true);
  
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
    
    // Update and render the graphics according to the physics
    m_graphics->Update(m_DT, m_physics, m_graphics->board);
    m_graphics->Update(m_DT, m_physics, m_graphics->cube);
    m_graphics->Render();

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
      m_graphics->cube->RBody->setLinearVelocity(btVector3(0, 0, 10));
      break;

      case SDLK_DOWN:
      m_graphics->cube->RBody->setLinearVelocity(btVector3(0, 0, -10));
      break;

      case SDLK_LEFT:
      m_graphics->cube->RBody->setLinearVelocity(btVector3(10, 0, 0));
      break;

      case SDLK_RIGHT:
      m_graphics->cube->RBody->setLinearVelocity(btVector3(-10, 0, 0));
      break;

      case SDLK_SPACE:
      m_graphics->cube->RBody->applyCentralImpulse(btVector3(0, 10, 0));
      break;

      default:
      m_graphics->cube->RBody->setLinearVelocity(btVector3(0, 0, 0));
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
      m_graphics->cube->RBody->setLinearVelocity(btVector3(0, 0, 0));
      break;

      case SDLK_DOWN:
      m_graphics->cube->RBody->setLinearVelocity(btVector3(0, 0, 0));
      break;

      case SDLK_LEFT:
      m_graphics->cube->RBody->setLinearVelocity(btVector3(0, 0, 0));
      break;

      case SDLK_RIGHT:
      m_graphics->cube->RBody->setLinearVelocity(btVector3(0, 0, 0));
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
