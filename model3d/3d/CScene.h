//
//  CScene.h
//  model3d
//
//  Created by 魏裕群 on 14-4-11.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__CScene__
#define __model3d__CScene__

#include "platform.h"

#include "Matrices.h"
#include "Vectors.h"
#include "CNode.h"
#include <vector>
using namespace std;
typedef struct camera{
    float x,y,z;
}Camera;

class CScene{
private:
    Camera m_sCamera;
    Matrix4 m_sProjection;
    vector<CNode*> m_iSubNodes;
public:
    void addSubNode(CNode* node);
    void reshape(int w,int h);
    void draw();
    inline const Matrix4& getProjection(){return m_sProjection;}
};

#endif /* defined(__model3d__CScene__) */
