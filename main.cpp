#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <time.h>
#include "Point.h"
#include "Train.h"
#include "Track.h"
using namespace std;

// width & height of our window
int width = 1500;
int height = 700;

clock_t t; // the time of last display

bool play = false;

Track track;
Train train1;

/***GLUT Callback Functions ******************************************************/

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:	// Esc key
			exit(0);
		case ' ':
			play ? play = false : play = true;
			break;
	}
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

	width = w;
	height = h;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(0.0, 3.0, 10.0,
			  0.0, 0.0, 0.0,
			  0.0, 1.0, 0.0);

	track.draw();
	train1.draw();
	//train2.draw();

	glutSwapBuffers();
}

void idle(void)
{
	double delta_t;		// the time span since last display

	// calculate time span since last display and reset time variable
	delta_t = (clock() - t) / (double)CLOCKS_PER_SEC;
	t = clock();

	if (!play)
		return;

	train1.integrate_motion(delta_t, track);
	//train2.integrate_motion(delta_t, track);

	// detect collision


	glutPostRedisplay();
}

/*********************************************************************************/

void init_anim(void)
{
	train1 = Train(track.pts[0].x(), track.pts[0].y(), track.pts[0].z());
	//train2.distance_travelled = 10.0;
}

void init_lighting(void)
{
	GLfloat light0_position[]	= { -4.0, 5.0, 10.0, 1.0 };
	GLfloat world_ambient[]		= { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_specular[]	= { 1.0, 1.0, 1.0, 1.0 };
	GLfloat material_specular[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat material_specular_exponent[] = { 80.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, world_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_specular_exponent);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);	
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width,height);
	glutInitWindowPosition(50,100);
	glutCreateWindow("Toy Train Set");

	init_anim();
	init_lighting();

	// callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();
}