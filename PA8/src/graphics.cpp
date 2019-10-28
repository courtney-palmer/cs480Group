#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height, int argc, char **argv)
{
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Create the objects

  cube = new Object("cubeTest.obj", box); // test object
  board = new Object("box.obj", plane);
  //declaring our box
  // floor = new Object("cubeTest.obj", box);
  // leftWall = new Object("cubeTest.obj", box);
  // rightWall = new Object("cubeTest.obj", box);
  // backWall = new Object("cubeTest.obj", box);
  // frontWall = new Object("cubeTest.obj", box);

  ball = new Object("sphere.obj", sphere);
  cylinder = new Object("cylinder.obj", cylind);

  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER, argc, argv))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER, argc, argv))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt, Physics *p, Object *o)
{
 // Update the object
  btTransform trans; //supports rigid movement like transformations and rotations
  btScalar m[16]; //4x4 matrix

  // note : for Graphics::Update, we may not need to use dt it seems?
  p->dynamicsWorld->stepSimulation(1.f/60.f, 10); //dictates how quick simulation runs
  p->Update();
  o->RBody->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);
  o->model = glm::make_mat4(m);

  // btTransform trans;
  // btScalar m[16];
  // physics.dynamicsWorld->stepSimulation(dt, 10);

  // object.RBody->getMotionState()->getWorldTransform(trans);

  // trans.getOpenGLMatrix(m);
  // for(int i = 0; i != 17; i++){
  //   std::cout << m[i] << std::endl;
  // }
  // object.model = glm::make_mat4(m);

  //object.Update(dt);
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  // Render the objects
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(cube->GetModel()));
  cube->Render();

  // Rendering a box
  // glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(floor->GetModel()));
  // floor->Render();

  // glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(leftWall->GetModel()));
  // leftWall->Render();

  // glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(rightWall->GetModel()));
  // rightWall->Render();

  // glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(backWall->GetModel()));
  // backWall->Render();

  // glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(frontWall->GetModel()));
  // frontWall->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(board->GetModel()));
  board->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(cylinder->GetModel()));
  cylinder->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(ball->GetModel()));
  ball->Render();

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    cout << "Error initializing OpenGL! " << error << ", " << val << endl;
  }
}

string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

