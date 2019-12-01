#ifndef ENGINE_H
#define ENGINE_H

#define GLM_ENABLE_EXPERIMENTAL
#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"
#include "camera.h"
#include "physics.h"



class Engine
{
  public:
    Engine(string name, int width, int height);
    Engine(string name);
    ~Engine();
    bool Initialize(char **argv);
    void Run();
    void Keyboard();
    unsigned int getDT();
    long long GetCurrentTimeMillis();

    int shaderToggle = 0;
    int buffer;
    int bufferMax;

    void outputObjects() const;
    int getIndexOf(const std::string& key); // Returns index from 0-objs.size();

  private:
    std::vector<Object*> objs;
    
    // Window related variables
    Window *m_window;    
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    SDL_Event m_event;

    Graphics *m_graphics;
    Physics *m_physics;
    unsigned int m_DT;
    long long m_currentTimeMillis;

    bool m_running;

    //Game Logic
    unsigned int score;
    unsigned int timer; //should be in milliseconds, to work with getDT()
    const unsigned int MAX_TIME = 60000; //ms
    bool playing; //determines whether the game is currently running - should the timer be counting down?

    int basketIndex, diskIndex;
};

#endif // ENGINE_H
