//
//  Stage.cpp
//  model3d
//
//  Created by 魏裕群 on 14-4-17.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#include "Stage.h"
#include "Dispatcher.h"
#include "BaseAnimation.h"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

double _x=0.0,_y=0.0;
bool _cap = false;
MotionEvent motionEvent;
KeyEvent keyEvent;

long Stage::sm_iCurrentTime = 0;
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
static void __init__()
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

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
	}
	keyEvent.key=key;
	keyEvent.scancode=scancode;
	keyEvent.action=action;
	keyEvent.mods=mods;
	Dispatcher::sharedInstance()->dispatcherKeyEvent(keyEvent);
}
static void cursor_pos_callback(GLFWwindow* window, double x, double y){
	//printf("pos:(%f,%f)\n",x,y);
	_x = x;
	_y = y;
	if (_cap) {
		motionEvent.x = _x;
		motionEvent.y = _y;
		motionEvent.action = MotionMove;
		Dispatcher::sharedInstance()->dispatcherTouchEvent(motionEvent);
	}
}
static void cursor_enter_callback(GLFWwindow* window, int x){
	if (_cap&&!x) {
		_cap = false;
		motionEvent.x = _x;
		motionEvent.y = _y;
		motionEvent.action = MotionCancel;
		Dispatcher::sharedInstance()->dispatcherTouchEvent(motionEvent);
	}
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods){
	motionEvent.x = _x;
	motionEvent.y = _y;
	if (button == GLFW_MOUSE_BUTTON_LEFT){
		if (action == GLFW_PRESS) {
			_cap = true;
			motionEvent.action=MotionDown;
		}else if(action == GLFW_RELEASE){
			_cap = false;
			motionEvent.action=MotionUp;
		}
		Dispatcher::sharedInstance()->dispatcherTouchEvent(motionEvent);
	}
}
Stage* Stage::sm_pSharedStage = 0;

Stage::Stage():m_lAnimationInterval(1.0f/60.0f),m_pWindow(nullptr),m_pCamera(nullptr){
    if (sm_pSharedStage) {
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
	glfwSetMouseButtonCallback(m_pWindow, mouse_callback);
	glfwSetKeyCallback(m_pWindow, key_callback);
	glfwSetCursorPosCallback(m_pWindow, cursor_pos_callback);
	glfwSetCursorEnterCallback(m_pWindow, cursor_enter_callback);
    glEnable(GL_DEPTH_TEST);
    __init__();
}

void Stage::addScene(Scene *scene){
    m_iScenes.push_back(scene);
}

Scene* Stage::runningScene(){
	return m_iScenes.size()>0?m_iScenes.back():nullptr;
}

int Stage::run(){
    LARGE_INTEGER nFreq;
	LARGE_INTEGER nLast;
	LARGE_INTEGER nNow;

	QueryPerformanceFrequency(&nFreq);
	
    Scene *top_scene = m_iScenes.size()>0?m_iScenes.back():nullptr;
    if (!top_scene) {
        return 1;
    }
	int width=0, height=0;

    while (!glfwWindowShouldClose(m_pWindow))
    {
		int nwidth, nheight;
		glfwGetFramebufferSize(m_pWindow, &nwidth, &nheight);
		if (nwidth!=width||nheight!=height) {
			width=nwidth;
			height=nheight;
			top_scene->screenSizeChange(width, height);
		}
		QueryPerformanceCounter(&nLast);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        top_scene->draw();
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
		QueryPerformanceCounter(&nNow);
		double dfMinus = (double)nNow.QuadPart - nLast.QuadPart;
		dfMinus/=nFreq.QuadPart;
		sm_iCurrentTime = (((float)nNow.QuadPart)/nFreq.QuadPart)*1000;
		for (int i=0; i<Animation::sm_nAnims.size(); i++) {
			Animation::sm_nAnims[i]->update();
		}
        if(dfMinus < m_lAnimationInterval){
            Sleep((m_lAnimationInterval - dfMinus)*1000);
        }
    }
    
    glfwDestroyWindow(m_pWindow);
    
    glfwTerminate();
	return 0;
}

int Stage::run(Scene *scene){
    addScene(scene);
    return run();
}
