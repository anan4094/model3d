#define GLUT_DISABLE_ATEXIT_HACK
 
#include "model3d.h"
CMesh mesh;
GLfloat rx=0,ry=0,rz=0;
void init()  
{  
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
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
    rx+=.05f;ry+=.05f;rz+=.05f;
    mesh.setRotation(rx, ry, rz);
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
	mesh.load("/Users/anan/Documents/github/model3d/res/cube.obj");
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
	glOrtho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
	glutDisplayFunc(display);  
	glutReshapeFunc(reshape);
    for (; 1; ) {
        glutCheckLoop();
        glutPostRedisplay();
    }
	glutMainLoop();
	return 0;  
}  