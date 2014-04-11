//
//  platform.h
//  model3d
//
//  Created by 魏裕群 on 14-4-11.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef model3d_platform_h
#define model3d_platform_h

#ifndef __APPLE__
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#else
#include <glew.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif

#endif
