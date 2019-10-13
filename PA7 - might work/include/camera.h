#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"
#include "window.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    void Update(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);



    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float cameraSpeed = 500.0f;
    
  private:
    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
