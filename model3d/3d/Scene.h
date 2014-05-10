//
//  CScene.h
//  model3d
//
//  Created by 魏裕群 on 14-4-11.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__Scene__
#define __model3d__Scene__

#include "platform.h"
#include "Node.h"
#include "Camera.h"

class Scene:public Node{
private:
    void reshape(int w,int h);
public:
    virtual bool screenSizeChange(int width,int height);
    void draw();
};

#endif /* defined(__model3d__Scene__) */
