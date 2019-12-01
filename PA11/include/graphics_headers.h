#ifndef GRAPHICS_HEADERS_H
#define GRAPHICS_HEADERS_H

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  //#include <GL/glu.h>
#endif

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

//Bullet
#include <btBulletDynamicsCommon.h>

// Image Magick
#include <Magick++.h>

#define INVALID_UNIFORM_LOCATION -1

struct Vertex
{
  glm::vec3 vertex;
  glm::vec3 color;
  glm::vec3 normal;
  glm::vec2 texture;

//Vertex(glm::vec3 v, glm::vec3 c, glm::vec3 n) :vertex(v), color(c), normal(n) {}
Vertex(glm::vec3 v, glm::vec3 c, glm::vec3 n, glm::vec2 t) :vertex(v), color(c), normal(n), texture(t) {}
};

enum Shape //for use with bullet
{
	box,
	sphere,
	plane,
	cylind,
	mesh,
	ghostObject_mesh
};

struct ShapeInfo { // Goes along with shape
  Shape shapeName;
  // 0 - default size value
  btScalar extents[3]; // stores up to 3 values for use of initializing primitives
  ShapeInfo(Shape s, float x=0, float y=0, float z=0) : shapeName(s) {
    extents[0] = x; extents[1] = y; extents[2] = z;
  }
  btVector3 getBtVector3() const {
    if(validify())
      return btVector3(extents[0], extents[1], extents[2]);
    else {
      return btVector3(0,0,0);
    } 
  }
  
  bool validify() const { // check extents values to make sure each are nonnegative values
    return (extents[0] >= 0 &&
	    extents[1] >= 0 &&
	    extents[2] >= 0) ?  true : false;
  }

  void output() const {
    std::cout << shapeName << " ";
    for(int i = 0; i < 3; i++) {
      std::cout << extents[i] << " ";
    }
    std::cout << std::endl;
  }
};



#endif /* GRAPHICS_HEADERS_H */
