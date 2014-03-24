#define GLUT_DISABLE_ATEXIT_HACK
 
#include "model3d.h"
CMesh mesh;
GLfloat rx=0,ry=0,rz=0;
void init()  
{  
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);  
}  
void draw_triangle()  
{  
	glBegin(GL_POLYGON);  
	glVertex3f(-4.0f, -4.0f, 4.0f);
	glVertex3f(4.0f, -4.0f, 4.0f);
	glVertex3f(4.0f, 4.0f, 4.0f);
	glVertex3f(-4.0f, 4.0f, 4.0f);  
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
    mesh.setRotation(rx, ry, rz);
	mesh.draw();
	//draw_triangle();
	glutSwapBuffers();
	glutPostRedisplay();
}  
void reshape(int w, int h)  
{  
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);  
}  
int main(int argc, char **argv)  
{  
	mesh.load("../res/cube.obj");
	mesh.log();
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  
	glutInitWindowPosition(600, 400);  
	glutInitWindowSize(400, 400);  
	glutCreateWindow("OpenGL Hello World");  
	init();  
	glColor3f(1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(-20.0f, 20.0f, -20.0f, 20.0f, -20.0f, 20.0f);
	glutDisplayFunc(display);  
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;  
}  