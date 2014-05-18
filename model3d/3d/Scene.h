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
#include "Light.h"
class Scene:public Node{
protected:
    vector<Light*> m_iLights;
private:
    void reshape(int w,int h);
public:
    virtual bool screenSizeChange(int width,int height);
    void addLight(Light* light);
	Light* getLight(int index){return m_iLights.size()>index?m_iLights[index]:nullptr;}
	int lightCount(){return m_iLights.size();}
    virtual void draw();
	virtual void clear();
};

#endif /* defined(__model3d__Scene__) */
