#include "Point.h"

Point::Point()
{

}

Point::Point(GLfloat x, GLfloat y, GLfloat z)
{
	set(x, y, z);
}

Point& Point::operator =(const Point &P)
{
	set(P.x_coord, P.y_coord, P.z_coord);

	return *this;
}

Point::~Point()
{

}

void Point::set(GLfloat x, GLfloat y, GLfloat z)
{
	setX(x);
	setY(y);
	setZ(z);
}

void Point::setX(GLfloat x)
{
	x_coord = x;
}

void Point::setY(GLfloat y)
{
	y_coord = y;
}

void Point::setZ(GLfloat z)
{
	z_coord = z;
}

GLfloat Point::x()
{
	return x_coord;
}

GLfloat Point::y()
{
	return y_coord;
}

GLfloat Point::z()
{
	return z_coord;
}

Point Point::cross(Point v1, Point v2)
{
	return Point(v1.y()*v2.z() - v1.z()*v2.y(),
				 v1.z()*v2.x() - v1.x()*v2.z(),
				 v1.x()*v2.y() - v1.y()*v2.x());
}

float Point::dot(Point v1, Point v2)
{
	return ( v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z() );
}

float Point::magnitude(void)
{
	return sqrt( pow(x_coord,2) + pow(y_coord,2) + pow(z_coord,2) );
}

void Point::normalize(void)
{	
	x_coord = x_coord / this->magnitude();
	y_coord = y_coord / this->magnitude();
	z_coord = z_coord / this->magnitude();
}

Point Point::operator +(const Point &rhs)
{
	return Point(x_coord + rhs.x_coord, y_coord + rhs.y_coord, z_coord + rhs.z_coord);
}

Point Point::operator -(const Point &rhs)
{
	return Point(x_coord - rhs.x_coord, y_coord - rhs.y_coord, z_coord - rhs.z_coord);
}

Point Point::operator *(const float num)
{
	return Point(x_coord * (GLfloat)num, y_coord * (GLfloat)num, z_coord * (GLfloat)num);
}

Point Point::operator /(const float num)
{
	return Point(x_coord / (GLfloat)num, y_coord / (GLfloat)num, z_coord / (GLfloat)num);
}


/* Yields the result of rotating the point (x,y,z) about the line through (a,b,c) with direction
 * vector (u,v,w) by the angle theta.
 *
 * | (a(v^2 + w^2) - u(bv + cw - ux - vy - wz))(1 - cos(theta)) + xcos(theta) + (-cv + bw - wy + vz)sin(theta) |
 * | (b(u^2 + w^2) - v(au + cw - ux - vy - wz))(1 - cos(theta)) + ycos(theta) + (cu - aw + wx - uz)sin(theta)  |
 * | (c(u^2 + v^2) - w(au + bv - ux - vy - wz))(1 - cos(theta)) + zcos(theta) + (-bu + av - vx + uy)sin(theta) |
 *
 * Parameters:
 *		'rP'	- the point we want to rotate						...(x,y,z)
 *		'dV'	- the direction vector (must be normalized)			...(u,v,w)
 *		'oP'	- the point the direction vector passes through		...(a,b,c)
 *		'theta'	- the angle we rotate the point by	
 *
 * Reference:
 *		http://inside.mines.edu/~gmurray/ArbitraryAxisRotation/
 */
Point Point::rotate_about_axis(Point rP, Point dV, Point oP, float theta)
{
	Point result;
	GLfloat x = rP.x(), y = rP.y(), z = rP.z();
	GLfloat u = dV.x(), v = dV.y(), w = dV.z();
	GLfloat a = oP.x(), b = oP.y(), c = oP.z();
	
	result.setX(	(a*(pow(v,2) + pow(w,2)) - u*(b*v + c*w - u*x - v*y - w*z))*(1 - cos(theta)) + x*cos(theta) + (-c*v + b*w - w*y + v*z)*sin(theta)	);
	result.setY(	(b*(pow(u,2) + pow(w,2)) - v*(a*u + c*w - u*x - v*y - w*z))*(1 - cos(theta)) + y*cos(theta) + (c*u - a*w + w*x - u*z)*sin(theta)	);
	result.setZ(	(c*(pow(u,2) + pow(v,2)) - w*(a*u + b*v - u*x - v*y - w*z))*(1 - cos(theta)) + z*cos(theta) + (-b*u + a*v - v*x + u*y)*sin(theta)	);

	return result;
}