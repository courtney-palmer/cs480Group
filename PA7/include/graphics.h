#ifndef GRAPHICS_H
#define GRAPHICS_H

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <fstream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, int argc, char **argv);
    void Update(unsigned int dt, int currentCase);
    void Render();

    vector<Object> planets;
    bool loadConfig();
    Camera *m_camera;
    glm::vec3 planetVecCoords(vector<Object> planets, std::string objectName);
    //loop through our vector of planets, return the model we are asking for
    Object getObject(vector<Object> planets, std::string objectName);
    int currentCase;
    void checkCameraImputs(int currentCase);


  private:
    std::string ErrorString(GLenum error);
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

};

#endif /* GRAPHICS_H */
