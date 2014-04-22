#define GLUT_DISABLE_ATEXIT_HACK
 
#include "Mesh.h"
#include "Stage.h"
#ifndef WIN32
#include <unistd.h>
#endif
Scene mainScene;
Mesh *pmesh=nullptr;

int main(int argc, char **argv)
{
    //char buf[1024];
    //getcwd(buf,1024);
    Stage *stage = new Stage();
    stage->init();
	//init();
    pmesh=new Mesh();
    //下面两个方法要在load后加载
    pmesh->setForceGenerateNormal(true);
    pmesh->setSmoothSurface(false);
#ifdef __APPLE__
    pmesh->load("/Users/anan/Documents/github/model3d/assert/peri/peri.obj");
#else
	pmesh->load("../assert/peri/peri.obj");
#endif
	pmesh->setUseShader(true);
	GLfloat hy = pmesh->getMaxY();
	GLfloat ly = pmesh->getMinY();
    pmesh->setRotationAxis(0, 1, 0);
    
    //let model show us
	GLfloat scale = 18/(hy-ly);
    pmesh->setScale(scale,scale,scale);
	pmesh->setPosition(0, -9.0f*(hy+ly)/(hy-ly), -20);
    
    mainScene.addSubNode(pmesh);

    stage->run(&mainScene);
    
    delete stage;
	delete pmesh;
	return 0;  
}