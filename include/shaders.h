#ifndef SHADERS_H
#define SHADERS_H

typedef unsigned int ShaderProgram;

ShaderProgram constructShaderProgramFromString(const char* vertexSource, const char* fragmentSource);
ShaderProgram constructShaderProgramFromFile(const char* vertexPath, const char* fragmentPath);

#endif
