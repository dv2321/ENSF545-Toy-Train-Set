#ifndef POINT_H
#define POINT_H

#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;

const double PI = 3.14159265;

class Point
{
public:
	Point();
	Point(GLfloat x, GLfloat y, GLfloat z);
	Point& operator =(const Point &P);
	~Point();

	void set(GLfloat x, GLfloat y, GLfloat z);
	void setX(GLfloat x);
	void setY(GLfloat y);
	void setZ(GLfloat z);
	GLfloat x();
	GLfloat y();
	GLfloat z();

	static Point cross(Point v1, Point v2);	// a vector perpendicular to v1 & v2
	static float dot(Point v1, Point v2);
	static Point rotate_about_axis(Point rP, Point dV, Point oP, float theta);
	float magnitude(void);					// vector magnitude
	void normalize(void);					// unit vector		

	Point operator +(const Point &rhs);
	Point operator -(const Point &rhs);
	Point operator *(const float num);
	Point operator /(const float num);

	friend ostream& operator <<(ostream& output, const Point& P)
	{
		output << P.x_coord << " " << P.y_coord << " " << P.z_coord;
		return output;
	}

	friend ofstream& operator <<(ofstream& output, const Point& P)
	{
		output << P.x_coord << " " << P.y_coord << " " << P.z_coord;
		return output;
	}
	friend ifstream& operator >>(ifstream& input, Point P)
	{
		input >> P.x_coord >> P.y_coord >> P.z_coord;
		return input;
	}

private:
	GLfloat x_coord;
	GLfloat y_coord;
	GLfloat z_coord;
};

#endif