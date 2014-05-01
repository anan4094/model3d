#define GLUT_DISABLE_ATEXIT_HACK
 
#include "Mesh.h"
#include "Stage.h"
#include "TestScene.h"
#ifndef WIN32
#include <unistd.h>
#endif
Mesh *pmesh=nullptr;

int main(int argc, char **argv)
{
    //char buf[1024];
    //getcwd(buf,1024);
    Stage *stage = new Stage();
    stage->init();
	//init();
    TestScene *mainScene = new TestScene();
	mainScene->initRes();

    stage->run(mainScene);
    
	delete mainScene;
    delete stage;
	return 0;
}