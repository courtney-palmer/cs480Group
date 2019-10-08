#include "camera.h"
#include "window.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}


void Camera::processInput(Window *window){
 

}


bool Camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  cameraPos   = glm::vec3(0.0f, 8.0f,  -16.0f);
  cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

  view = glm::lookAt( cameraPos, cameraFront, cameraUp); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane, 
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}


