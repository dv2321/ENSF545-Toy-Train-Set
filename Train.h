#ifndef TRAIN_H
#define TRAIN_H

#include <GL/glut.h>
#include <math.h>
#include "Point.h"
#include "Track.h"
using namespace std;

class Train
{
public:
	Train();
	Train(GLfloat x, GLfloat y, GLfloat z);
	Train& operator =(const Train &car);
	~Train();

	void integrate_motion(double delta_t, Track &track);
	void draw(void);

	Point position;

	float distance_travelled;
	float velocity;
	float acceleration;
	float damping;

private:
	// Frenet Frame
	Point T;			// tangent vector pointint in the direction of velocity
	Point vi_T;			// previous velocity tanget
	Point N;			// normal vector
	Point B;			// bi-normal vector
	GLfloat frenet[16];
};

#endif