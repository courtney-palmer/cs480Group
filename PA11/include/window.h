#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <string>

using namespace std;

#define BACKGROUND_MUSIC "Assets/Sounds/Electronic-ambient-background-beat.mp3"

class Window
{
  public:
    Window();
    ~Window();
    bool Initialize(const string &name, int* width, int* height);
    void Swap();
    static Mix_Music *backgroundMusic;

		void PlayMusic(bool isPlaying);
		bool isPlayingMusic = true;

  private:
    SDL_Window* gWindow;
    SDL_GLContext gContext;
};

#endif /* WINDOW_H */
