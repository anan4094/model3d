//
//  Stage.cpp
//  model3d
//
//  Created by 魏裕群 on 14-4-17.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#import "Stage.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>

//用来检查OpenGL版本，需要GLSL 2.0支持
static void getGlVersion( int *major, int *minor )
{
    const char* verstr = (const char*)glGetString( GL_VERSION );
    if( (verstr == NULL) || (sscanf( verstr, "%d.%d", major, minor ) != 2) )
    {
        *major = *minor = 0;
        fprintf( stderr, "Invalid GL_VERSION format!!!\n" );
    }
}
static void init()
{
    glewExperimental = true;
    GLenum err = glewInit();
    if (err != GLEW_OK ){
        printf("Error: %s\n", glewGetErrorString(err));
    }
    int gl_major, gl_minor;
    // Make sure that OpenGL 2.0 is supported by the driver
    getGlVersion(&gl_major, &gl_minor);
    printf("GL_VERSION major=%d minor=%d\n", gl_major, gl_minor);
    if (gl_major < 2)
    {
        printf("GL_VERSION major=%d minor=%d\n", gl_major, gl_minor);
        printf("Support for OpenGL 2.0 is required for this demo...exiting\n");
        exit(1);
    }
    
	glClearColor(0.9f, 0.9f, 0.9f, 0.0f);
	glShadeModel(GL_SMOOTH);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE , GL_FALSE);
}
static long getCurrentMillSecond()
{
    long lLastTime = 0;
    struct timeval stCurrentTime;
    
    gettimeofday(&stCurrentTime,NULL);
    lLastTime = stCurrentTime.tv_sec*1000+stCurrentTime.tv_usec*0.001; //millseconds
    return lLastTime;
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

Stage* Stage::sm_pSharedStage = 0;

Stage::Stage():m_lAnimationInterval(1.0f/60.0f*1000.0f),m_pWindow(nullptr){
    if (!sm_pSharedStage) {
        return;
    }
    sm_pSharedStage = this;
}

Stage* Stage::sharedInstance(){
    if (!sm_pSharedStage) {
        
    }
    return sm_pSharedStage;
}

Stage::~Stage(){
    if (this!=sm_pSharedStage) {
        return;
    }
    sm_pSharedStage = 0;
}

void Stage::init(){
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    m_pWindow = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!m_pWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(m_pWindow);
    
    glfwSetKeyCallback(m_pWindow, key_callback);
    glEnable(GL_DEPTH_TEST);
    init();
}

void Stage::addScene(Scene *scene){
    m_iScenes.push_back(scene);
}

int Stage::run(){
    long lastTime = 0L;
    long curTime = 0L;
    Scene *top_scene = m_iScenes.size()>0?m_iScenes.back():nullptr;
    if (!top_scene) {
        return 1;
    }
    while (!glfwWindowShouldClose(m_pWindow))
    {
        int width, height;
        glfwGetFramebufferSize(m_pWindow, &width, &height);
        top_scene->reshape(width, height);
        lastTime = getCurrentMillSecond();
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        top_scene->draw();
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
        curTime = getCurrentMillSecond();
        if (curTime - lastTime < m_lAnimationInterval){
            usleep(static_cast<useconds_t>((m_lAnimationInterval - curTime + lastTime)*1000));
        }
    }
    
    glfwDestroyWindow(m_pWindow);
    
    glfwTerminate();    return 0;
}

int Stage::run(Scene *scene){
    addScene(scene);
    return run();
}
