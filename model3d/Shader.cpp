//
//  shader.cpp
//  model3d
//
//  Created by 魏裕群 on 14-3-28.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "Shader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

Shader::Shader():VertexShaderObject(0),FragmentShaderObject(0),ProgramObject(0){
}

Shader::~Shader(){
    if (ProgramObject) {
        glDeleteProgram(ProgramObject);
        ProgramObject = 0;
    }
    if (VertexShaderObject) {
        glDeleteShader(VertexShaderObject);
        VertexShaderObject = 0;
    }
    if (FragmentShaderObject) {
        glDeleteShader(FragmentShaderObject);
        FragmentShaderObject = 0;
    }
}

GLuint Shader::get(){
    return ProgramObject;
}

//Shader related functions
int Shader::shaderSize(const char *fileName, EShaderType shaderType)
{
    //返回顶点着色器或者片段着色器的大小
    char name[100];
    strcpy(name, fileName);
    
    switch (shaderType)
    {
        case EVertexShader:
            strcat(name, ".vert");
            break;
        case EFragmentShader:
            strcat(name, ".frag");
            break;
        default:
            printf("ERROR: unknown shader file type\n");
            return -1;
            break;
    }
    
    int count = -1;
    // Open the file, seek to the end to find its length
    // Open the file, seek to the end to find its length
    int fd = open(name, O_RDONLY);
    struct stat stat;
    if (fd != -1)
    {
        count = (int)((fstat(fd,&stat) < 0) ? -1 : stat.st_size);
        //count = _lseek(fd, 0, SEEK_END) + 1;
    }
    return count;
}

int Shader::readShader(const char *fileName, EShaderType shaderType, int size)
{
    //
    // Reads a shader from the supplied file and returns the shader in the
    // arrays passed in. Returns 1 if successful, 0 if an error occurred.
    // The parameter size is an upper limit of the amount of bytes to read.
    // It is ok for it to be too big.
    //
    FILE *fh;
    char *shaderText;
    char name[100];
    int count;
    
    strcpy(name, fileName);
    
    switch (shaderType)
    {
        case EVertexShader:
            strcat(name, ".vert");
            shaderText=(char*)VertexShaderSource;
            break;
        case EFragmentShader:
            strcat(name, ".frag");
            shaderText=(char*)FragmentShaderSource;
            break;
        default:
            printf("ERROR: unknown shader file type\n");
            return 0;
            break;
    }
    
    // Open the file
    fh = fopen(name, "r");
    if (!fh)
        return -1;
    
    // Get the shader from a file.
    fseek(fh, 0, SEEK_SET);
    count = (int) fread(shaderText, 1, size, fh);
    shaderText[count] = '\0';
    
    if (ferror(fh))
        count = 0;
    
    fclose(fh);
    return count;
}

int Shader::readShaderSource(const char *fileName)
{
    int vSize, fSize;
    
    //
    // Allocate memory to hold the source of our shaders.
    //
    vSize = shaderSize(fileName, EVertexShader);
    fSize = shaderSize(fileName, EFragmentShader);
    
    if ((vSize == -1) || (fSize == -1))
    {
        printf("Cannot determine size of the shader %s\n", fileName);
        return 0;
    }
    
    VertexShaderSource = (GLchar *) malloc(vSize);
    FragmentShaderSource = (GLchar *) malloc(fSize);
    
    //
    // Read the source code
    //
    if (!readShader(fileName, EVertexShader, vSize))
    {
        printf("Cannot read the file %s.vert\n", fileName);
        return 0;
    }
    
    if (!readShader(fileName, EFragmentShader, fSize))
    {
        printf("Cannot read the file %s.frag\n", fileName);
        return 0;
    }
    
    return 1;
}

void Shader::log(GLuint obj){
    GLint log_length=0;
    if (glIsShader(obj)) {
        glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&log_length);
    }else if (glIsProgram(obj)){
        glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&log_length);
    }else{
        printf("not a shader or a program\n");
        return;
    }
    char *log=(char*)malloc(log_length);
    if (glIsShader(obj)) {
        glGetShaderInfoLog(obj,log_length,nullptr,log);
    }else if(glIsProgram(obj)){
        glGetProgramInfoLog(obj,log_length,nullptr,log);
    }
    printf("%s",log);
    free(log);
}

int Shader::build(){
    GLint vertCompiled, fragCompiled;    // status values
    GLint linked;
    
    // Create a vertex shader object and a fragment shader object
    
    VertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    FragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Load source code strings into shaders
    
    glShaderSource(VertexShaderObject, 1, &VertexShaderSource, NULL);
    glShaderSource(FragmentShaderObject, 1, &FragmentShaderSource, NULL);
    
    // Compile the brick vertex shader, and print out
    // the compiler log file.
    
    glCompileShader(VertexShaderObject);
    glGetShaderiv(VertexShaderObject, GL_COMPILE_STATUS, &vertCompiled);
    
    // Compile the brick vertex shader, and print out
    // the compiler log file.
    
    glCompileShader(FragmentShaderObject);
    glGetShaderiv(FragmentShaderObject, GL_COMPILE_STATUS, &fragCompiled);
    
    if (!vertCompiled || !fragCompiled){
        if (vertCompiled==GL_FALSE) {
            log(VertexShaderObject);
        }
        if (fragCompiled==GL_FALSE) {
            log(FragmentShaderObject);
        }
        glDeleteShader(VertexShaderObject);
        VertexShaderObject = 0;
        glDeleteShader(FragmentShaderObject);
        FragmentShaderObject = 0;
        return 0;
    }
    
    // Create a program object and attach the two compiled shaders
    
    ProgramObject = glCreateProgram();
    glAttachShader(ProgramObject, VertexShaderObject);
    glAttachShader(ProgramObject, FragmentShaderObject);
    
    installAttrib();
    // Link the program object and print out the info log
    
    glLinkProgram(ProgramObject);
    glGetProgramiv(ProgramObject, GL_LINK_STATUS, &linked);
    
    if (!linked){
        log(ProgramObject);
        return 0;
    }
    
    // Install program object as part of current state
    
    glUseProgram(ProgramObject);
    installUniform();
    glDetachShader(ProgramObject,VertexShaderObject);
    glDetachShader(ProgramObject,FragmentShaderObject);
    
    return 1;
}

int Shader::use(){
    if (!ProgramObject) {
        return 0;
    }
    glUseProgram(ProgramObject);
    return 1;
}
