#ifndef ENGINE_H
#define ENGINE_H

#define GLM_ENABLE_EXPERIMENTAL
#include <sys/time.h>
#include <assert.h>

#include <ctime>
#include <cstdlib>

#include"BulletCollision/CollisionDispatch/btGhostObject.h"
#include "window.h"
#include "graphics.h"
#include "camera.h"
#include "physics.h"
#include "sound.h"

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

    void outputObjects() const;
    int getIndexOf(const std::string& key); // Returns index from 0-objs.size();

    std::vector<Object*> objs;
    std::vector<Object*> disks;

    Sound objectCollidedSound;

    void createObject(const std::string& objFileName, const ShapeInfo& newShape,
		      const std::string& key, const std::string& texFileName,
		      const float& x, const float& y, const float& z, const float& Rtype);

    // adds to disk vector instead
    void createDisk(const std::string& objFileName, const ShapeInfo& newShape,
		    const std::string& key, const std::string& texFileName,
		    const float& x, const float& y, const float& z, const float& Rtype);

    // Simplify random spawn of disk
    void spawnDisk();

    // Handles object deletion for physics and from engine array
    void deleteObject(std::vector<Object*>& objArray, int objIndex);

    //void deleteDisk(Object* disk);

    // Clears all object arrays, except for basket in obj
    void clearObjects();

    void loadLevel(int level = 0);
    // void loadLevel(std::string levelFile);

    void increaseScore(int amount);

  private:

    // Window related variables
    Window *m_window;    
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    SDL_Event m_event;

    Sound *m_sound;

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
    bool levelLoaded;
    int maxDisks; //maximum number of disks

    void deleteOutOfBoundsDisks();

    int basketIndex;
};

#endif // ENGINE_H
