#define GLUT_DISABLE_ATEXIT_HACK
 
#include "model3d.h"
CMesh mesh;
void init()  
{  
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  
	glShadeModel(GL_SMOOTH);  
}  
void draw_triangle()  
{  
	glBegin(GL_POLYGON);  
	glVertex3f(-4.0f, -4.0f, -4.0f);  
	glVertex3f(4.0f, -4.0f, -4.0f);  
	glVertex3f(4.0f, 4.0f, -4.0f);  
	glVertex3f(-4.0f, 4.0f, -4.0f);  
	glEnd();  
}  
void display()  
{  
	glClear(GL_COLOR_BUFFER_BIT);
	mesh.draw();
	//draw_triangle();  
	glutSwapBuffers();
}  
void reshape(int w, int h)  
{  
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);  
}  
int main(int argc, char **argv)  
{  
	mesh.load("C:\\Users\\anan\\Desktop\\cube.obj");
	mesh.log();
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  
	glutInitWindowPosition(600, 400);  
	glutInitWindowSize(400, 400);  
	glutCreateWindow("OpenGL Hello World");  
	init();  
	glColor3f(1.0f, 1.0f, 1.0f);  
	glOrtho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f);  
	glutDisplayFunc(display);  
	glutReshapeFunc(reshape);  
	glutMainLoop();
	return 0;  
}  