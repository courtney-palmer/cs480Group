
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
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, argc, argv))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

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

    // Update and render the graphics
    m_graphics->Update(m_DT, m_graphics->currentCase);
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

      //controls for controlling the camera
      case SDLK_1:
        m_graphics->currentCase = 1;
        break;
      
      case SDLK_2:
        m_graphics->currentCase = 2;
        break;
      
      case SDLK_3:
        m_graphics->currentCase = 3;
        break;
      
      case SDLK_4:
        m_graphics->currentCase = 4;
        break;

      case SDLK_5:
        m_graphics->currentCase = 5;
        break;

      case SDLK_6:
        m_graphics->currentCase = 6;
        break;
  
      case SDLK_7:
        m_graphics->currentCase = 7;
        break;
  
      case SDLK_8:
        m_graphics->currentCase = 8;
        break;
  
      case SDLK_9:
        m_graphics->currentCase = 9;
        break;
  
      case SDLK_0:
        m_graphics->currentCase = 0;
        break;
    
      case SDLK_q:
        m_graphics->currentCase = 10;
        break;
      
      case SDLK_w:
        m_graphics->currentCase = 11;
        break;

      case SDLK_e:
        m_graphics->currentCase = 12;
        break;
        
      //controls for controlling simulation speed
      case SDLK_UP:
        m_graphics->ChangeSimSpeed(true);
        break;

      case SDLK_DOWN:
        m_graphics->ChangeSimSpeed(false);
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
