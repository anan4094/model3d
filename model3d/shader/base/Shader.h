//
//  shader.h
//  model3d
//
//  Created by 魏裕群 on 14-3-28.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__shader__
#define __model3d__shader__
#ifndef __APPLE__
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#else
#include <glew.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif

#ifdef WIN32
#define SHADER_PATH "../resource/shader/"
#else
#define SHADER_PATH "/Users/anan/Documents/github/model3d/resource/shader/"
#endif

typedef enum{
    attrib_position,
    attrib_normal,
    attrib_color,
    attrib_texcoord0,
    attrib_texcoord1
}VertexAttrib;

typedef enum {
    EVertexShader,
    EFragmentShader,
} EShaderType;

class Shader{
protected:
    GLuint ProgramObject;
    GLuint VertexShaderObject;
    GLuint FragmentShaderObject;
    const GLchar *VertexShaderSource, *FragmentShaderSource;
    int shaderSize(const char *fileName, EShaderType shaderType);
    int readShader(const char *fileName, EShaderType shaderType, int size);
    void log(GLuint obj);
    virtual void installAttrib()  = 0;
    virtual void installUniform() = 0;
public:
    Shader();
    ~Shader();
    GLuint get();
    int readShaderSource(const char *fileName);
    int build();
    int use();
};

#endif /* defined(__model3d__shader__) */
