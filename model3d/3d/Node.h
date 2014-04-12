//
//  CNode.h
//  model3d
//
//  Created by 魏裕群 on 14-4-11.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__Node__
#define __model3d__Node__

class Scene;

class Node{
protected:
    Scene    *m_piScene;
public:
    inline void setScene(Scene*scene){m_piScene = scene;}
    virtual void draw() = 0;
};

#endif /* defined(__model3d__Node__) */
