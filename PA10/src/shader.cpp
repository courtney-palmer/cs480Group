#include <fstream>
#include <string>

#include "shader.h"

Shader::Shader()
{
  m_shaderProg = 0;
}

Shader::~Shader()
{
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin() ; it != m_shaderObjList.end() ; it++)
  {
    glDeleteShader(*it);
  }

  if (m_shaderProg != 0)
  {
    glDeleteProgram(m_shaderProg);
    m_shaderProg = 0;
  }
}

bool Shader::Initialize()
{
  m_shaderProg = glCreateProgram();

  if (m_shaderProg == 0) 
  {
    std::cerr << "Error creating shader program\n";
    return false;
  }

  return true;
}

// void Shader::toggleShader(GLenum ShaderType, int shaderToggle){
//   std::string s;

//   // shaderToggle seems to hold 0 if current shader is..? hm. idk
//   if(ShaderType == GL_VERTEX_SHADER && shaderToggle == 0)
//   {
//     std::cout << "VERT TOGGLE 0" << std::endl;
//     std::ifstream shaderFile ("../Assets/Shaders/fLight-shader-v.txt"); //open the file
//     if (!shaderFile) //if unable to open the file, write an error to the terminal
//       std::cerr << "There was a problem with the fragment shader file." << std::endl;
//     else
//       {
// 	//assign the text in the file to the string s
// 	s.assign((std::istreambuf_iterator<char>(shaderFile)), (std::istreambuf_iterator<char>()));
// 	shaderFile.close();
//       }
//   }
//   else if(ShaderType == GL_VERTEX_SHADER && shaderToggle == 1)
//   {
//     std::cout << "VERT TOGGLE 1" << std::endl;
//     std::ifstream shaderFile ("../Assets/Shaders/vLight-shader-v.txt"); //open the file
//     if (!shaderFile) //if unable to open the file, write an error to the terminal
//       std::cerr << "There was a problem with the fragment shader file." << std::endl;
//     else
//       {
// 	//assign the text in the file to the string s
// 	s.assign((std::istreambuf_iterator<char>(shaderFile)), (std::istreambuf_iterator<char>()));
// 	shaderFile.close();
//       }
//   }
//   else if(ShaderType == GL_FRAGMENT_SHADER && shaderToggle == 0)
//   {
//     std::cout << "FRAG TOGGLE 0" << std::endl;
//     std::ifstream shaderFile ("../Assets/Shaders/fLight-shader-f.txt"); //open the file
//     if (!shaderFile) //if unable to open the file, write an error to the terminal
//       std::cerr << "There was a problem with the fragment shader file." << std::endl;
//     else
//       {
// 	//assign the text in the file to the string s
// 	s.assign((std::istreambuf_iterator<char>(shaderFile)), (std::istreambuf_iterator<char>()));
// 	shaderFile.close();
//       }
//   }
//   else if(ShaderType == GL_FRAGMENT_SHADER && shaderToggle == 1)
//   {
//     std::cout << "FRAG TOGGLE 1" << std::endl;
//     std::ifstream shaderFile ("../Assets/Shaders/vLight-shader-f.txt"); //open the file
//     if (!shaderFile) //if unable to open the file, write an error to the terminal
//       std::cerr << "There was a problem with the fragment shader file." << std::endl;
//     else
//       {
// 	//assign the text in the file to the string s
// 	s.assign((std::istreambuf_iterator<char>(shaderFile)), (std::istreambuf_iterator<char>()));
// 	shaderFile.close();
//       }
//   }

//   // toggle is set, now add shaders
//   GLuint ShaderObj = glCreateShader(ShaderType);

//   if (ShaderObj == 0) 
//   {
//     std::cerr << "Error creating shader type " << ShaderType << std::endl;
//   }

//   // Save the shader object - will be deleted in the destructor
//   m_shaderObjList.push_back(ShaderObj);

//   const GLchar* p[1];
//   p[0] = s.c_str();
//   GLint Lengths[1] = { (GLint)s.size() };

//   glShaderSource(ShaderObj, 1, p, Lengths);

//   glCompileShader(ShaderObj);

//   GLint success;
//   glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

//   if (!success) 
//   {
//     GLchar InfoLog[1024];
//     glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
//     std::cerr << "Error compiling: " << InfoLog << std::endl;
//   }

//   glAttachShader(m_shaderProg, ShaderObj);
// }

// // Use this method to add shaders to the program. When finished - call finalize()
// bool Shader::AddShader(GLenum ShaderType, char **argv)
// {
//   std::string s;
// 	int i = 0;
//   if(ShaderType == GL_VERTEX_SHADER)
//   {
// 		while(!(strcmp(argv[i], "-v") == 0)) //go through arguments until you find -v flag
// 			i++;
// 		i++; //next argument is the file name we want
// 		std::string fileName(argv[i]);
// 		std::ifstream shaderFile ("../Assets/Shaders/" + fileName); //open the file
// 		if (!shaderFile) //if unable to open the file, write an error to the terminal
// 			std::cerr << "There was a problem with the vertex shader file." << std::endl;
// 		else
// 		{
// 			//assign the text in the file to the string s
// 			s.assign((std::istreambuf_iterator<char>(shaderFile)), (std::istreambuf_iterator<char>()));
// 			shaderFile.close();
// 		}
//   }
//   else if(ShaderType == GL_FRAGMENT_SHADER)
//   {
// 		while(!(strcmp(argv[i], "-f") == 0)) //go through arguments until you find -f flag
// 			i++;
// 		i++; //next argument is the file name we want
// 		std::string fileName(argv[i]);
// 		std::ifstream shaderFile ("../Assets/Shaders/" + fileName); //open the file
// 		if (!shaderFile) //if unable to open the file, write an error to the terminal
// 			std::cerr << "There was a problem with the fragment shader file." << std::endl;
// 		else
// 		{
// 			//assign the text in the file to the string s
// 			s.assign((std::istreambuf_iterator<char>(shaderFile)), (std::istreambuf_iterator<char>()));
// 			shaderFile.close();
// 		}
//   }

//   GLuint ShaderObj = glCreateShader(ShaderType);

//   if (ShaderObj == 0) 
//   {
//     std::cerr << "Error creating shader type " << ShaderType << std::endl;
//     return false;
//   }

//   // Save the shader object - will be deleted in the destructor
//   m_shaderObjList.push_back(ShaderObj);

//   const GLchar* p[1];
//   p[0] = s.c_str();
//   GLint Lengths[1] = { (GLint)s.size() };

//   glShaderSource(ShaderObj, 1, p, Lengths);

//   glCompileShader(ShaderObj);

//   GLint success;
//   glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

//   if (!success) 
//   {
//     GLchar InfoLog[1024];
//     glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
//     std::cerr << "Error compiling: " << InfoLog << std::endl;
//     return false;
//   }

//   glAttachShader(m_shaderProg, ShaderObj);

//   return true;
// }

bool Shader::AddShader(GLenum ShaderType, std::string filename) {

  std::string s; // This will hold the shader program code

  // Move the file information into s
  std::ifstream shaderFile("../Assets/Shaders/" + filename); // Open file
  // If file open, assign to string that holds info
  if(shaderFile) {
    s.assign((std::istreambuf_iterator<char>(shaderFile)), (std::istreambuf_iterator<char>()));
    shaderFile.close();
  }
  else {
    std::cerr << "There was a problem with loading the shader file." << std::endl;
    return false;
  }

  GLuint ShaderObj = glCreateShader(ShaderType);

  if(ShaderObj == 0) {
    std::cerr << "Error creating shader type " << ShaderType << std::endl;
    return false;
  }

  // Save shader object - will delete in destructor
  m_shaderObjList.push_back(ShaderObj);

  const GLchar* p[1];
  p[0] = s.c_str();
  GLint Lengths[1] = { (GLint)s.size() };

  glShaderSource(ShaderObj, 1, p, Lengths);

  glCompileShader(ShaderObj);

  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) 
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    std::cerr << "Error compiling: " << InfoLog << std::endl;
    return false;
  }

  glAttachShader(m_shaderProg, ShaderObj);

  return true;
}


// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::Finalize()
{
  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  glLinkProgram(m_shaderProg);

  glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
  if (Success == 0)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
    return false;
  }

  glValidateProgram(m_shaderProg);
  glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
    return false;
  }

  // Delete the intermediate shader objects that have been added to the program
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
  {
    glDeleteShader(*it);
  }

  m_shaderObjList.clear();

  return true;
}


void Shader::Enable()
{
    glUseProgram(m_shaderProg);
}


GLint Shader::GetUniformLocation(const char* pUniformName)
{
    GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (Location == INVALID_UNIFORM_LOCATION) {
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return Location;
}
