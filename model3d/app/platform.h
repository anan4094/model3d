//
//  platform.h
//  model3d
//
//  Created by 魏裕群 on 14-4-11.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef model3d_platform_h
#define model3d_platform_h

#ifdef WIN32
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#else
//#include <glew.h>
//#include <OpenGL/gl.h>
//#include <GLUT/glut.h>
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>
#import <OpenGL/glext.h>
#include "glfw3.h"
#endif


#ifdef __APPLE__
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#endif

#endif
