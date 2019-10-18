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
  loadConfig();
  //std::cout << "number of planets: " << planets.size() << std::endl;
  
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

	/**
    This function will loop through a given vector of objects and return an object based on a given name
    @param vector<Object> planets: this represents our vector of planets to be passed in.
    @param string objectName: this is the name of our planet, used to identify an object in our vector
    @pre none
    @post Will return an object corresponding to its name
    @return an object
	*/
Object Graphics::getObject(vector<Object> planets, std::string objectName){
  for(int i = 0; i < planets.size(); i++) {
      if(planets[i].getKey() == objectName){
        return planets[i];
      }
  }
}


	/**
    This function will format a position as a vector given an object type
    @param vector<Object> planets: this represents our vector of planets to be passed in.
    @param string objectName: this is the name of our planet, used to identify an object in our vector
    @pre none
    @post Will created a vector given an object
    @return a vector representing the object's current position
	*/
glm::vec3 Graphics::planetVecCoords(vector<Object> planets, std::string objectName){
  Object tempObject = getObject(planets, objectName);
  glm::mat4 tempModel = tempObject.GetModel();
  glm::vec3 tempVec(tempModel[3]);
  return tempVec;
}

	/**
    This function will focus a camera position given the state of currentCase
    @param int currentCase: this is an int (set by keyboard input in engine.cpp). this is used to swich camera view
    @pre none
    @post Will change camera view based on the status of currentCase
	*/
void Graphics::checkCameraImputs(int currentCase){
  switch(currentCase){
    case 0:{
      std::string planetName = "Sun";
      glm::vec3 planetVec = planetVecCoords(planets, planetName);
      m_camera->Update(planetVec.x, planetVec.y + 1.0f, planetVec.z - 20.0f,
		       planetVec.x, planetVec.y, planetVec.z,
		       0.0f, 1.0f, 0.0f);
    }
      break;
    case 1:{
      std::string planetName = "Mercury";
      glm::vec3 planetVec = planetVecCoords(planets, planetName);
      m_camera->Update(planetVec.x, planetVec.y + 0.5f, planetVec.z - 1.0f,
		       planetVec.x, planetVec.y, planetVec.z,
		       0.0f, 1.0f, 0.0f);
    }
      break;
    case 2:{
      std::string planetName = "Venus";
      glm::vec3 planetVec = planetVecCoords(planets, planetName);
      m_camera->Update(planetVec.x, planetVec.y + 1.0f, planetVec.z - 3.0f,
		       planetVec.x, planetVec.y, planetVec.z,
		       0.0f, 1.0f, 0.0f);
    }
      break;
    case 3:{
      std::string planetName = "Earth";
      glm::vec3 planetVec = planetVecCoords(planets, planetName);
      m_camera->Update(planetVec.x, planetVec.y + 1.0f, planetVec.z - 3.0f,
		       planetVec.x, planetVec.y , planetVec.z,
		       0.0f, 1.0f, 0.0f);
      // changed 2nd argument to tilt camera upwards a bit so the orbit path
      // doesn't look like bullets shooting through the planets
    }
      break;
    case 4:{
      std::string planetName = "Mars";
      glm::vec3 planetVec = planetVecCoords(planets, planetName);
      m_camera->Update(planetVec.x, planetVec.y + 1.0f, planetVec.z - 5.0f,
		       planetVec.x, planetVec.y, planetVec.z,
		       0.0f, 1.0f, 0.0f);
    }
      break;
    case 5:{
      std::string planetName = "Jupiter";
      glm::vec3 planetVec = planetVecCoords(planets, planetName);
      m_camera->Update(planetVec.x, planetVec.y + 5.0f, planetVec.z - 25.0f,
		       planetVec.x, planetVec.y, planetVec.z,
		       0.0f, 1.0f, 0.0f);

    }
      break;
    case 6:{
      std::string planetName = "Saturn";
      glm::vec3 planetVec = planetVecCoords(planets, planetName);
      m_camera->Update(planetVec.x, planetVec.y + 1.5f, planetVec.z - 16.0f,
		       planetVec.x, planetVec.y, planetVec.z,
		       0.0f, 1.0f, 0.0f);
    }
      break;
    case 7:{
      std::string planetName = "Uranus";
      glm::vec3 planetVec = planetVecCoords(planets, planetName);
      m_camera->Update(planetVec.x, planetVec.y + 1.0f, planetVec.z - 18.0f,
		       planetVec.x, planetVec.y, planetVec.z,
		       0.0f, 1.0f, 0.0f);
    }
      break;
    case 8:{
      std::string planetName = "Neptune";
      glm::vec3 planetVec = planetVecCoords(planets, planetName);
      m_camera->Update(planetVec.x, planetVec.y + 1.0f, planetVec.z - 10.0f,
		       planetVec.x, planetVec.y, planetVec.z,
		       0.0f, 1.0f, 0.0f);
    }
      break;
    case 9:{
      std::string planetName = "Pluto";
      glm::vec3 planetVec = planetVecCoords(planets, planetName);
      m_camera->Update(planetVec.x, planetVec.y + 0.5f, planetVec.z - 0.5f,
		       planetVec.x, planetVec.y, planetVec.z,
		       0.0f, 1.0f, 0.0f);
    }
      break;
    case 10:{ // q
      m_camera->Update(0.0f ,8.0f ,-32.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    }
      break;
    case 11:{ // w
      m_camera->Update(0.0f ,8.0f ,-96.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    }
      break;
    case 12:{ // e
      m_camera->Update(0.0f ,100.0f ,-220.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    }
      break;
  default: 
      m_camera->Update(0.0f ,8.0f ,-96.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
      break;
  }
}

void Graphics::Update(unsigned int dt, int currentCase)
{
  // Update the object
  for(int i = 0; i < planets.size() ; i++) {
    glm::mat4 orbitOrigin;
    if(planets[i].getOriginIndex() == -1)
    {
      orbitOrigin = glm::mat4(1.0f);
    }
    else
    {
      orbitOrigin = planets[planets[i].getOriginIndex()].GetPosition();
    }
    planets[i].Update(dt, orbitOrigin);
  }
  checkCameraImputs(currentCase);
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
  for(int i = 0; i < planets.size(); i++) {
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(planets[i].GetModel()));
    planets[i].Render();
  }

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    cout << "Error initializing OpenGL! " << error << ", " << val << endl;
  }
}

bool Graphics::loadConfig() {

  // Open .config File
  std::fstream configFile("../config.txt");
  if(!configFile.is_open()) {
    cout << "Failed to open config." << endl;
    return false;
  }

  // Skip the first two lines
  std::string line;
  int skipLinesTo = 2;
  int i = 1; 
  while(!configFile.eof() && i <= skipLinesTo) {
    i++;
    getline(configFile, line);
  }

  // Now process data in config file, by storing it into planets
  // Clear planets first, just in case
  planets.clear();

  // Data to get per line
  string key, modelfile, texturefile;
  float scale, speed, rotationSpeed, orbitRadius;
  int origin;

  while(!configFile.eof()) {
    // store config file line data into data values
    
    configFile >> key >> modelfile >> texturefile;
    configFile >> scale >> speed >> rotationSpeed >> orbitRadius >> origin;

    if(configFile.eof())
      break;
  /*
    cout << "keyname: " << key << " " << modelfile << " " << texturefile << endl
	 << "scale: " << scale << endl
	 << "speed: " << speed << endl
	 << "rotation speed: " << rotationSpeed << endl
	 << "orbit radius: " << orbitRadius << endl
	 << "origin: " << origin << endl << endl;
  */

    // initialize new planet
    planets.push_back( Object(modelfile, texturefile, key, origin,
			      scale, speed, rotationSpeed, orbitRadius) );
  }
  

  configFile.close();
  return true;
}

void Graphics::ChangeSimSpeed(bool scalar)
{
  for(int i = 0; i < planets.size(); i++)
  {
    planets[i].SetOrbitSpeed(scalar);
    planets[i].SetSpinSpeed(scalar);
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

