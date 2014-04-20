#define GLUT_DISABLE_ATEXIT_HACK
 
#include "Mesh.h"
#include "Stage.h"
#ifndef WIN32
#include <unistd.h>
#endif
Scene mainScene;
Mesh *pmesh=nullptr;
GLfloat rad=0;
bool loop=true;
void draw_triangle()  
{  
	glBegin(GL_POLYGON);  
	glVertex3f(-4.0f, -4.0f, -20.0f);
	glVertex3f(4.0f, -4.0f, -20.0f);
	glVertex3f(4.0f, 4.0f, -20.0f);
	glVertex3f(-4.0f, 4.0f, -20.0f);
	glEnd();  
}  
void display(GLFWwindow*window)
{  
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    rad+=.1f;
	static const GLfloat light_position[] = {20.0f, 20.0f, 20.0f, 0.0f};
	static const GLfloat light_ambient[]   = {0.1f, 0.1f, 0.1f, 1.0f};
	static const GLfloat light_diffuse[]   = {1.0f, 1.0f, 1.0f, 1.0f};
	static const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION,   light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT,   light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,   light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
    GLfloat matAmbient[] = {0.6f, 0.6f, 0.6f, 1.0f};
    GLfloat matDiffuse[]   = {0.35f, 0.35f, 0.35f, 1.0f};
    GLfloat matAmbDif[]   = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat matSpecular[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat shine[] = {5.0f};
    GLfloat matEmission[] = {0.3f, 0.1f, 0.1f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbDif);  // 将背景颜色和散射颜色设置成同一颜色
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shine);
    glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);  // 用来模拟物体发光的效果，但这不是光源
    
    pmesh->setRadian(rad);
	mainScene.draw();
	//draw_triangle();
	glfwSwapBuffers(window);
    glfwPollEvents();
}
void reshape(int w, int h)  
{  
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glFrustum(-1.0f, 1.0f, -1.0f*w/h, 1.0f*w/h,1.0f, 40.0f);
    mainScene.reshape(w, h);
}

void keyboard(unsigned char key,int x,int y){
    if (key=='q'||key==0x1b) {
        loop = false;
    }
}
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
    //mesh.load("/Users/anan/Documents/github/model3d/assert/female/female.obj");
    //pmesh->load("/Users/anan/Documents/github/model3d/assert/tails/tails.obj");
    pmesh->load("/Users/anan/Documents/github/model3d/assert/peri/peri.obj");
#else
	//pmesh->load("../assert/tails/tails.obj");
	//pmesh->load("../assert/female/female.obj");
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