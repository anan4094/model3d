#define GLUT_DISABLE_ATEXIT_HACK
 
#include "model3d.h"
#include <unistd.h>
CMesh mesh;
GLfloat rx=0,ry=0,rz=0;
void init()  
{  
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE , GL_FALSE);
}  
void draw_triangle()  
{  
	glBegin(GL_POLYGON);  
	glVertex3f(-4.0f, -4.0f, -20.0f);
	glVertex3f(4.0f, -4.0f, -20.0f);
	glVertex3f(4.0f, 4.0f, -20.0f);
	glVertex3f(-4.0f, 4.0f, -20.0f);
	glEnd();  
}  
void display()  
{  
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    rx+=.3f;ry+=.3f;rz+=.3f;
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
    GLfloat matAmbient[] = {0.6, 0.6, 0.6, 1.0};
    GLfloat matDiffuse[]   = {0.35, 0.35, 0.35, 1.0};
    GLfloat matAmbDif[]   = {0.5, 0.5, 0.5, 1.0};
    GLfloat matSpecular[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat shine[] = {5.0};
    GLfloat matEmission[] = {0.3, 0.1, 0.1, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbDif);  // 将背景颜色和散射颜色设置成同一颜色
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shine);
    glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);  // 用来模拟物体发光的效果，但这不是光源
    
    mesh.setRotation(rx, ry, rz);
	mesh.draw();
	//draw_triangle();
	glutSwapBuffers();
	glutPostRedisplay();
}  
void reshape(int w, int h)  
{  
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glFrustum(-1.0f, 1.0f, -1.0f*w/h, 1.0f*w/h,1.0f, 40.0f);
}  
int main(int argc, char **argv)  
{
    char buf[1024];
    getcwd(buf,1024);
#ifdef __APPLE__
    mesh.load("/Users/anan/Documents/github/model3d/res/cube.obj");
#else
	mesh.load("../res/cube.obj");
#endif
	//mesh.log();
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB|GLUT_DEPTH);
	glutInitWindowPosition(600, 400);  
	glutInitWindowSize(400, 400);  
	glutCreateWindow("OpenGL Hello World");  
	init();  
	glColor3f(1.0f, 1.0f, 1.0f);
	glutDisplayFunc(display);  
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;  
}  