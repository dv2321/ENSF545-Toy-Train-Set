#include "Train.h"

Train::Train()
{

}

Train::Train(GLfloat x, GLfloat y, GLfloat z)
{
	position.set(x, y, z);
	distance_travelled = 0.0;
	velocity = 0.0;
	acceleration = 0.0;
	damping = 0.08;


}

Train& Train::operator =(const Train &car)
{
	position = car.position;
	distance_travelled = car.distance_travelled;
	velocity = car.velocity;
	acceleration = car.acceleration;
	damping = car.damping;

	return *this;
}

Train::~Train()
{

}

void Train::integrate_motion(double delta_t, Track &track)
{ 
	// locate current position on track
	int segment_index = 0;
	bool moving_forward = track.find_arc_segment(distance_travelled, segment_index);

	// calculate slope of the segment this train is currently located on
	float theta = track.calculate_slope(segment_index);

	acceleration = 9.81 * sin(theta)								// force due to gravity
				 + (velocity >= 0 ? -1 : 1) * damping * 9.81 * cos(theta);	// force due to friction
	velocity += acceleration * delta_t;
	distance_travelled += velocity * delta_t;

	if (distance_travelled > track.total_length)
		distance_travelled -= track.total_length;
	else if (distance_travelled < 0)
		distance_travelled += track.total_length;

	//cout << "a: " << acceleration << endl;
	//cout << "v: " << velocity << endl;
	//cout << "d: " << distance_travelled << endl;

	/* Frenet Frame orientation *****************/
	track.find_arc_segment(distance_travelled, segment_index);

	// calculate position on arc segment
	float ds = track.calculate_ds(distance_travelled, segment_index);
	T = track.pts[(segment_index == track.pts.size()-1) ? 0 : segment_index+1] - track.pts[segment_index];

	T = T / T.magnitude(); // normalize
	Point Y(0, 1, 0); // vector pointing in the direction of positive (+) y-axis
	B = Point::cross(T, Y);
	B = B / B.magnitude(); // normalize
	N = Point::cross(B, T);
	N = N / N.magnitude(); // normalize

	Point v = T * ds;
	position = v + track.pts[segment_index]; // position along the track
	position = position + (N * 0.55);		 // elevate slightly above the track

	// put into matrix for OpenGL to do the calculation for us
	frenet[0] = N.x(); frenet[1] = N.y(); frenet[2] = N.z(); frenet[3] = 0;
	frenet[4] = T.x(); frenet[5] = T.y(); frenet[6] = T.z(); frenet[7] = 0;
	frenet[8] = B.x(); frenet[9] = B.y(); frenet[10]= B.z(); frenet[11]= 0;
	frenet[12]= position.x(); frenet[13]=position.y(); frenet[14]=position.z(); frenet[15]= 1;
}

void Train::draw(void)
{
	glPushMatrix();
		glMultMatrixf(frenet);
		glColor3f(0.2, 0.0, 1.0);
		glutSolidCube(1.0);
	glPopMatrix();
}