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

#include "Matrices.h"
#include "Vectors.h"
#include "Node.h"

class Scene:public Node{
private:
    Matrix4 m_sProjection;
    
    void reshape(int w,int h);
public:
    virtual bool screenSizeChange(int width,int height);
    void draw();
    inline const Matrix4& getProjection(){return m_sProjection;}
};

#endif /* defined(__model3d__Scene__) */
