#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);

    void Update(float posX, float posY, float posZ,
		float frontX, float frontY, float frontZ,
		float upX, float upY, float upZ);

    glm::mat4 GetProjection();
    glm::mat4 GetView();
  
  private:
    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
