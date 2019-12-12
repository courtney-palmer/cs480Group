#include <window.h>

//Mix_Music* Window::backgroundMusic;

Window::Window()
{
  gWindow = NULL;
}

Window::~Window()
{
  SDL_StopTextInput();
  //Mix_FreeMusic(Window::backgroundMusic);
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  SDL_Quit();
}

// void Window::PlayMusic(bool isPlaying)
// {
//   if(isPlaying)
//   {
//     Mix_PlayMusic(backgroundMusic,-1);;
//     isPlayingMusic = true;
//   } else
//   {
//     Mix_HaltMusic();
//     isPlayingMusic = false;
//   }
// }

bool Window::Initialize(const string &name, int* width, int* height)
{
    // Start SDL
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL failed to initialize: %s\n", SDL_GetError());
    return false;
  }

  // if (MIX_INIT_MP3 != (Mix_Init(MIX_INIT_MP3))) {
  //   printf("Could not initialize mixer (result:).\n");
  //   printf("Mix_Init: %s\n", Mix_GetError());
    // return false;
  //}

  // Sound Initialization
  // Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
  // Window::backgroundMusic = Mix_LoadMUS(BACKGROUND_MUSIC);
  // Mix_AllocateChannels(16);
  // Mix_PlayMusic(backgroundMusic,-1);

  // Start OpenGL for SDL
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );

  // Create window
  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);
  
  //use for fullscreen
  if (*height == 0 && *width == 0)
  {
   // uncomment for full screen
    *height = current.h;
    *width = current.w;

    // uncomment the following for windowed size
    //*height = 600;
     //*width = 800;
  }

  gWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, *width, *height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
  if(gWindow == NULL)
  {
    printf("Widow failed to create: %s\n", SDL_GetError());
    return false;
  }

  // Create context
  gContext = SDL_GL_CreateContext(gWindow);
  if(gContext == NULL)
  {
    printf("OpenGL context not created: %s\n", SDL_GetError());
    return false;
  }

  // Use VSync
  if(SDL_GL_SetSwapInterval(1) < 0)
  {
    printf("Unable to use VSync: %s\n", SDL_GetError());
    return false;
  }

  return true;
}

void Window::Swap()
{
  SDL_GL_SwapWindow(gWindow);
}
