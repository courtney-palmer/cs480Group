#ifndef SHADER_H
#define SHADER_H

#include <vector>

#include "graphics_headers.h"

class Shader
{
  public:
    Shader();
    ~Shader();
    bool Initialize();
    void Enable();
    bool AddShader(GLenum ShaderType, char **argv);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);

    void toggleShader(GLenum ShaderType, int shaderToggle);

    GLuint m_shaderProg;    // both were origionally private. Leaving as public temporarily
    std::vector<GLuint> m_shaderObjList;
};

#endif  /* SHADER_H */
