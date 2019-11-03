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
#include <vector>

class Graphics
{
  friend class Physics;
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, char **argv);
    void Update(Physics *p, Object *o);
    void Render();
    void Render(std::vector<Object*>& objs);

    Object *board; //static
    Object *cylinder; //static
    Object *cube; //dynamic
    Object *ball; //dynamic
    Object *floor; // static
    Object *leftWall; // static
    Object *rightWall; // static
    Object *backWall; // static
    Object *frontWall; // static
    Shader *m_shader;

    glm::vec3 ambience = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 specular  = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 diffuse  = glm::vec3(0.05f, 0.05f, 0.05f);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

};

#endif /* GRAPHICS_H */
