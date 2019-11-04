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

    void toggleShader(int tog);

    void RenderLight(Shader* shader, glm::vec4 lPos, glm::vec4 ambProd, glm::vec4 difProd, glm::vec4 specProd, float shine);
    Object *board; //static
    Object *cylinder; //static
    Object *cube; //dynamic
    Object *ball; //dynamic
    
    std::vector<Shader*> shaders;
    Shader *m_shader;

    Object *lamp;

    glm::vec3 ambience = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 specular  = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuse  = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

 struct Light{
    glm::vec3 direction;
    glm::vec3 position;
    float cutoff;
    
    glm::vec3 ambience;
    glm::vec3 diffuse;
    glm::vec3 spectular;

    float constant;
    float linear;
    float quadratic;
  };

    Light spotLight;
    float spotLightAngle;
    float theta;

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

};

 #endif /* GRAPHICS_H */
