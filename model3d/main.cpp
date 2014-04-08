#define GLUT_DISABLE_ATEXIT_HACK
 
#include "CMesh.h"
#ifndef WIN32
#include <unistd.h>
#endif
CMesh mesh;
GLfloat rad=0;
bool loop=true;
//用来检查OpenGL版本，需要GLSL 2.0支持
void getGlVersion( int *major, int *minor )
{
    const char* verstr = (const char*)glGetString( GL_VERSION );
    if( (verstr == NULL) || (sscanf( verstr, "%d.%d", major, minor ) != 2) )
    {
        *major = *minor = 0;
        fprintf( stderr, "Invalid GL_VERSION format!!!\n" );
    }
}
void init()  
{
    glewExperimental = true;
    GLenum err = glewInit();
    if (err != GLEW_OK ){
        printf("Error: %s\n", glewGetErrorString(err));
    }
    int gl_major, gl_minor;
    // Make sure that OpenGL 2.0 is supported by the driver
    getGlVersion(&gl_major, &gl_minor);
    printf("GL_VERSION major=%d minor=%d\n", gl_major, gl_minor);
    if (gl_major < 2)
    {
        printf("GL_VERSION major=%d minor=%d\n", gl_major, gl_minor);
        printf("Support for OpenGL 2.0 is required for this demo...exiting\n");
        exit(1);
    }

	glClearColor(0.9f, 0.9f, 0.9f, 0.0f);
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
    
    mesh.setRadian(rad);
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

void keyboard(unsigned char key,int x,int y){
    if (key=='q') {
        loop = false;
    }
}
int main(int argc, char **argv)
{
    char buf[1024];
    //getcwd(buf,1024);
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB|GLUT_DEPTH);
	glutInitWindowPosition(600, 400);  
	glutInitWindowSize(480, 480);
#ifdef WIN32
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
	glutCreateWindow("OpenGL Hello World");

	init();
    //下面两个方法要在load后加载
    mesh.setForceGenerateNormal(true);
    mesh.setSmoothSurface(true);
#ifdef __APPLE__
    mesh.load("/Users/anan/Documents/github/model3d/res/tails/Tails.obj");
#else
	mesh.load("../res/female/female.obj");
#endif
    //
    mesh.initShader();
    mesh.setRotationAxis(0, 1, 0);
    
    //let famele.obj show us
    //mesh.setScale(0.02, 0.02, 0.02);
	//mesh.setPosition(0, -16, -20);

    //let tails.obj show us
    mesh.setScale(2.4f, 2.4f, 2.4f);
	mesh.setPosition(0, -8, -20);
    
	glColor3f(1.0f, 0.0f, 1.0f);
	glutDisplayFunc(display);  
	glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
	while (loop) {
#ifdef __APPLE__
		glutCheckLoop();
#else
		glutMainLoopEvent();
#endif
    }
	return 0;  
}  