//
//  Stage.cpp
//  model3d
//
//  Created by 魏裕群 on 14-4-17.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#import "Stage.h"
#include "Event.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>

double _x=0.0,_y=0.0;
bool _cap = false;
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
static void cursor_pos_callback(GLFWwindow* window, double x, double y){
    //printf("pos:(%f,%f)\n",x,y);
    _x = x;
    _y = y;
    if (_cap) {
        Stage::sharedInstance()->runningScene()->dispatcherTouchEvent(touchMove, _x, _y);
    }
}
static void cursor_enter_callback(GLFWwindow* window, int x){
    if (_cap&&!x) {
        _cap = false;
        Stage::sharedInstance()->runningScene()->dispatcherTouchEvent(touchEnd, _x, _y);
    }
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_LEFT){
        if (action == GLFW_PRESS) {
            _cap = true;
            Stage::sharedInstance()->runningScene()->dispatcherTouchEvent(touchBegin, _x, _y);
        }else if(action == GLFW_RELEASE){
            _cap = false;
            Stage::sharedInstance()->runningScene()->dispatcherTouchEvent(touchEnd, _x, _y);
        }
    }
}

Stage* Stage::sm_pSharedStage = 0;

Stage::Stage():m_lAnimationInterval(1.0f/60.0f*1000.0f),m_pWindow(nullptr){
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
}

void Stage::addScene(Scene *scene){
    m_iScenes.push_back(scene);
}

Scene* Stage::runningScene(){
    return m_iScenes.size()>0?m_iScenes.back():nullptr;
}

int Stage::run(){
    long lastTime = 0L;
    long curTime = 0L;
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