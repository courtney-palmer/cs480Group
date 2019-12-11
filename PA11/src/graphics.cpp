#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

/*
  Constructor
  @param int width: width of the window
  @param int height: height of the window
  @param char** argv: command line arguments with shader.txt file names
  @pre n/a
  @post Initializes shaders, all objects
*/
bool Graphics::Initialize(int width, int height, char **argv)
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

  // for binding light
  GLuint LightVao;
  glGenVertexArrays(1, &LightVao);
  glBindVertexArray(LightVao);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  shaderIndex = 0;

  // Load Shaders
  if(!loadShaders(argv)) {
    std::cout << "Shaders Failed to Initialize" << std::endl;
  }
  // DEBUG : also load default shaders
  addShaders("shader-v.txt", "shader-f.txt");

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

/*
  This loads the shader from given command line arguments
  @param char** argv: pointers to c-strings holding shader .txt names
  @pre argv is not empty
  @post shaders vector is initialized
  @UsedIn Graphics::Initialize()
 */
bool Graphics::loadShaders(char **argv) {
  // === Set up the shaders ===

  int numShaders = 0; //number of shader programs -> 1 vert shader + 1 frag shader = 1 shader program
  int v, f; //index of v flag and f flag
  int i = 0;

  while(!(strcmp(argv[i], "-v") == 0)) //go through arguments until you find -v flag
		i++;
  v = i; //set v flag's index
  i++; //next argument is first shader
  while(!(strcmp(argv[i], "-f") == 0)) //go through arguments until you find -f flag
  {
    i++;
    numShaders++; //each argument is a new shader program
  }
  f = i; //set f flag's index
	
  for(i = 1; i < numShaders + 1; i++)
  {
    shaders.push_back(new Shader());
    if(!shaders.back()->Initialize())
    {
      printf("Shader Failed to Initialize\n");
      return false;
    }

    std::string fileName(argv[v + i]); //file we want is at the v flag plus an offset of i (i = 1 for the first shader, i = 2 for the second, etc)
    if(!shaders.back()->AddShader(GL_VERTEX_SHADER, fileName))
    {
      printf("Vertex Shader failed to Initialize\n");
      return false;
    }

    fileName.clear();

    fileName.assign(argv[f + i]); //file we want is at the f flag plus an offset of i (i = 1 for the first shader, i = 2 for the second, etc)
    // Add the fragment shader
    if(!shaders.back()->AddShader(GL_FRAGMENT_SHADER, fileName))
    {
      printf("Fragment Shader failed to Initialize\n");
      return false;
    }

    // Connect the program
    if(!shaders.back()->Finalize())
    {
      printf("Program unable to Finalize\n");
      return false;
    }
  }

  m_shader = shaders[shaderIndex];
  return true;
}

/*
  Function adds shaders to the list of shaders available
 */
bool Graphics::addShaders(std::string vFileName, std::string fFileName) {
  shaders.push_back(new Shader());

  if(!shaders.back()->Initialize()) {
    std::cout << "Shader failed to initialize." << std::endl;
    shaders.pop_back();
    return false;
  }

  // Vertex shader
  if(!shaders.back()->AddShader(GL_VERTEX_SHADER, vFileName)) {
    std::cout << "Vertex shader failed init\n";
    shaders.pop_back();
    return false;
  }

  // Frag shader
  if(!shaders.back()->AddShader(GL_FRAGMENT_SHADER, fFileName)) {
    std::cout << "Fragment shader failed init\n";
    shaders.pop_back();
    return false;
  }

  // Finalize
  if(!shaders.back()->Finalize()) {
    std::cout << "Shader program unable to finalize.\n";
    shaders.pop_back();
    return false;
  }

  return true;
}

//void Graphics::Update(Physics *p, Object *o)
void Graphics::Update(Object *o)
{
 // Update the object
  btTransform trans; //supports rigid movement like transformations and rotations
  btScalar m[16]; //4x4 matrix

  o->RBody->getMotionState()->getWorldTransform(trans);
  trans.getOpenGLMatrix(m);
  o->model = glm::make_mat4(m);
}

void Graphics::Render(std::vector<Object*>& objs)
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
  for(int i = 0; i < objs.size(); i++) {
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(objs[i]->GetModel()));
    if(objs[i]->getKeyname() == "glassTop" ){ // is object is our "ceiling", do not render it
      //objs[i]->Render(1); //render 5 instances of this object
    }
    else{
      objs[i]->Render(1); //otherwise render each object once
    }
  }

  //////////////////////////////// -- RENDERING LIGHT THNIGS -- ////////////////////////////////

  if(shaderIndex != 2) {
    glUniform4f(m_shader->GetUniformLocation("LightPosition"),  lightPos.x, lightPos.y, lightPos.z, 0.0f);
    glUniform4f(m_shader->GetUniformLocation("AmbientProduct"), ambience.x, ambience.y, ambience.z, 1.0f);
    glUniform4f(m_shader->GetUniformLocation("DiffuseProduct"), diffuse.x, diffuse.y, diffuse.z, 1.0f);
    glUniform4f(m_shader->GetUniformLocation("SpecularProduct"), specular.x, specular.y, specular.z, 1.0f);
    glUniform1f(m_shader->GetUniformLocation("Shininess"), shininess);
  }
  //////////////////////////////// -- END OF RENDERING LIGHT THNIGS -- ////////////////////////////////

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

void Graphics::toggleShader(int tog)
{
  // Check if tog is within range
  if(tog < shaders.size() && tog >= 0) {
    shaderIndex = tog;
    m_shader = shaders[tog];
  }
  else
    std::cout << "Not a valid shader toggle value." << std::endl;
}

