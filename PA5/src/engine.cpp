
#include "engine.h"

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
    m_graphics->Update(m_DT);
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

		//planet controls
		case SDLK_q:
		m_graphics->planet->reversedOrbit = !(m_graphics->planet->reversedOrbit); //make the planet reverse its orbit
		break;

		case SDLK_e:
		m_graphics->planet->pausedOrbit = !(m_graphics->planet->pausedOrbit); //make the planet pause/unpause its orbit
		break;

		case SDLK_z:
		m_graphics->planet->reversedSpin = !(m_graphics->planet->reversedSpin); //make planet reverse spin
		break;

		case SDLK_c:
		m_graphics->planet->pausedSpin = !(m_graphics->planet->pausedSpin); //make planet pause/unpause its spin
		break;

		case SDLK_w:
		m_graphics->planet->SetScale(true); //increase scale of planet
		break;

		case SDLK_s:
		m_graphics->planet->SetScale(false); //decrease scale of planet
		break;

		case SDLK_d:
		m_graphics->planet->SetOrbitSpeed(true); //increase speed of planet orbit
		break;

		case SDLK_a:
		m_graphics->planet->SetOrbitSpeed(false); //decrease speed of planet orbit
		break;

		case SDLK_r:
		m_graphics->planet->SetSpinSpeed(true); //increase speed of planet spin
		break;

		case SDLK_t:
		m_graphics->planet->SetSpinSpeed(false); //decrease speed of planet spin
		break;

		//moon controls
		case SDLK_i:
		m_graphics->moon->reversedOrbit = !(m_graphics->moon->reversedOrbit); //make the moon reverse its orbit
		break;

		case SDLK_p:
		m_graphics->moon->pausedOrbit = !(m_graphics->moon->pausedOrbit); //make the moon pause/unpause its orbit
		break;

		case SDLK_n:
		m_graphics->moon->reversedSpin = !(m_graphics->moon->reversedSpin); //make moon reverse spin
		break;

		case SDLK_m:
		m_graphics->moon->pausedSpin = !(m_graphics->moon->pausedSpin); //make moon pause/unpause its spin
		break;

		case SDLK_o:
		m_graphics->moon->SetScale(true); //increase scale of moon
		break;

		case SDLK_l:
		m_graphics->moon->SetScale(false); //decrease scale of moon
		break;

		case SDLK_SEMICOLON:
		m_graphics->moon->SetOrbitSpeed(true); //increase speed of moon orbit
		break;

		case SDLK_k:
		m_graphics->moon->SetOrbitSpeed(false); //decrease speed of moon orbit
		break;

		case SDLK_y:
		m_graphics->moon->SetSpinSpeed(true); //increase speed of moon spin
		break;

		case SDLK_u:
		m_graphics->moon->SetSpinSpeed(false); //decrease speed of moon spin
		break;

		default:
		break;
	}
  }
  else if (m_event.type == SDL_MOUSEBUTTONDOWN)
  {
    // handle key down events here
	switch(m_event.button.button)
	{
		case SDL_BUTTON_LEFT: //left mouse button
		m_graphics->planet->pausedOrbit = !(m_graphics->planet->pausedOrbit); //make the planet pause/unpause its orbit
		break;

		case SDL_BUTTON_RIGHT: //right mouse button
		m_graphics->moon->pausedOrbit = !(m_graphics->moon->pausedOrbit); //make the moon pause/unpause its orbit
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
