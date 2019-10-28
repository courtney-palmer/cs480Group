#ifndef GRAPHICS_H
#define GRAPHICS_H

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "physics.h"

class Graphics
{
  friend class Physics;
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, int argc, char **argv);
    void Update(unsigned int dt, Physics *p, Object *o);
    void Render();

    Object *board; //static
    Object *cylinder; //static
    Object *cube; //dynamic
    Object *ball; //dynamic
    Object *floor;
    Object *leftWall;
    Object *rightWall;
    Object *backWall;
    Object *frontWall;
  
  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

};

#endif /* GRAPHICS_H */
