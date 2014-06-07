//
//  Stage.h
//  model3d
//
//  Created by 魏裕群 on 14-4-17.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__Stage_mac__
#define __model3d__Stage_mac__
#include "Scene.h"
#include <vector>
class Stage{
public:
    Stage();
    virtual ~Stage();
    void init();
    void addScene(Scene*scene);
    Scene *runningScene();
	inline Camera *currentCamera(){return m_pCamera;}
	inline void setCamera(Camera *pCamera){m_pCamera=pCamera;}
    int run(Scene*scene);
    int run();
    static Stage* sharedInstance();
    static long                sm_iCurrentTime;
protected:
    double m_lAnimationInterval;  //micro second
    GLFWwindow                 *m_pWindow;
    std::vector<Scene*>        m_iScenes;
	Camera                     *m_pCamera;
    static Stage               *sm_pSharedStage;
    
};
#endif /* defined(__model3d__Stage_mac__) */
