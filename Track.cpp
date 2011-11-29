#include "Track.h"

Track::Track()
{
	import_track_pts();
	build_track();

	/* Incremental arc lengths of all the cubic segments are calculated and 
	 * stored in a vector (size = pts.size() + 1)
	 */	
	Point direction_vector;
	total_length = 0.0;
	inc_arc_length_segments.push_back(total_length);
	for (int i = 0; i < pts.size(); i++)
	{
		if (i == pts.size() - 1) // last point
			direction_vector = pts[0] - pts[i];
		else
			direction_vector = pts[i+1] - pts[i];

		total_length += direction_vector.magnitude();
		inc_arc_length_segments.push_back(total_length);
	}
}

Track::~Track()
{

}

void Track::cubic_mask(vector<Point> &fine_points, vector<Point> &coarse_points)
{
	/*
	 *	Cubic B-Splines mask { 1/8, 1/2, 3/4, 1/2, 1/8 }
	 */
	int N = coarse_points.size();

	// closed curve - loop back to last coarse point
	fine_points.push_back(coarse_points[N-1]*0.125 + coarse_points[0]*0.75 + coarse_points[1]*0.125);
    fine_points.push_back(coarse_points[0]*0.5 + coarse_points[1]*0.5);

	// periodic mask for the interior points
	for (int i = 1; i < N-1; i++)
	{
		fine_points.push_back(coarse_points[i-1]*0.125 + coarse_points[i]*0.75 + coarse_points[i+1]*0.125);
        fine_points.push_back(coarse_points[i]*0.5 + coarse_points[i+1]*0.5);
	}

	// closed curve - loop back to first coarse point
	fine_points.push_back(coarse_points[N-2]*0.125 + coarse_points[N-1]*0.75 + coarse_points[0]*0.125);
    fine_points.push_back(coarse_points[N-1]*0.5 + coarse_points[0]*0.5);
}

void Track::cubic_BSpline(vector<Point> &fine_points, vector<Point> &control_points, int num_steps)
{
	if (!control_points.empty())
	{
		vector<Point> coarse_points;

		for (int i = 0; i < num_steps; i++)
		{
			// first iteration - find the fine points assuming the coarse points are the control points
			if (i == 0)
			{
				fine_points.clear();
				cubic_mask(fine_points, control_points);
			}
			// otherwise - assume the coarse points are the previous fine points and then find new fine points
			else
			{
				coarse_points = fine_points;
				fine_points.clear();
				cubic_mask(fine_points, coarse_points);
			}
		}
	}
}

bool Track::find_arc_segment(float distance_travelled, int &segment_index)
{
	bool moving_forward = true;

	if (distance_travelled >= inc_arc_length_segments[segment_index])		// track car moved forward
	{
		moving_forward = true;
		for (int i = segment_index; i < inc_arc_length_segments.size()-1; i++)		
			if (distance_travelled >= inc_arc_length_segments[i] &&
					distance_travelled < inc_arc_length_segments[i+1]) {
				
				segment_index = i; 
				break;
			}
	}
	else if (distance_travelled < inc_arc_length_segments[segment_index])	// track car moved backwards	
	{
		moving_forward = false;
		for (int i = segment_index-1; i > 0; i--)
			if (distance_travelled >= inc_arc_length_segments[i] &&
					distance_travelled < inc_arc_length_segments[i+1]) {

				segment_index = i;
				break;
			}
	}

	return moving_forward;
}

float Track::calculate_slope(int segment_index)
{
	float theta, height, length;

	height = pts[segment_index].y() - pts[(segment_index == pts.size()-1) ? 0 : segment_index+1].y();
	length = inc_arc_length_segments[(segment_index == pts.size()-1) ? 0 : segment_index+1] - inc_arc_length_segments[segment_index];

	theta = asin(height/length);

	return theta;
}

float Track::calculate_ds(float distance_travelled, int segment_index)
{
	return distance_travelled - inc_arc_length_segments[segment_index];
}

void Track::import_track_pts(void)
{
	ifstream in;
	float x, y, z;
	in.open("control_points.txt");
	if (!in.good())
	{
		cout << "Error opening file. Terminating program." << endl;
		exit(1);
	}	
	// read each line of the file
	while (!in.eof())
	{
		char buf[20];
		char *token;
		in.getline(buf, 20);		
		// parse the line 
		token = strtok(buf, " ");
		while (token != NULL)
		{
			x = atof(token); token = strtok(NULL, " ");
			y = atof(token); token = strtok(NULL, " ");
			z = atof(token); token = strtok(NULL, " ");

			ctrl_pts.push_back(Point(x,y,z));
		}
	}
	in.close();

	cout << "imported points..." << endl;
	for (int i = 0; i < ctrl_pts.size(); i++)
		cout << ctrl_pts[i] << endl;

	// build track - cubic B spline with 8 iterations
	cubic_BSpline(pts, ctrl_pts, 8);	
}

void Track::build_track(void)
{
	Point tangent, prev_tangent, normal, bi_normal; // vectors
	Point left_rail, right_rail, middle_rail, cross_rail;
	
	for (int i = 0; i < pts.size(); i++)
	{
		tangent = (i == pts.size()-1 ? pts[0] : pts[i+1]) - pts[i];		
		tangent = tangent / tangent.magnitude(); // normalize

		prev_tangent = pts[i] - (i == 0 ? pts.back() : pts[i-1]);
		prev_tangent = prev_tangent / prev_tangent.magnitude(); // normalize

		Point Y(0,1,0);	// take a vector pointing in the direction of positive (+) y-axis

		bi_normal = Point::cross(tangent, Y);

		normal = Point::cross(bi_normal, tangent);
		normal = normal / normal.magnitude(); // normalize

		bi_normal = bi_normal / bi_normal.magnitude();
		bi_normal = bi_normal * 0.2;
		right_rail = bi_normal + pts[i];
				
		bi_normal = bi_normal / bi_normal.magnitude();
		bi_normal = bi_normal * (-0.2);
		left_rail = bi_normal + pts[i];

		normal = normal / normal.magnitude();
		normal = normal * (-0.08);
		middle_rail = normal + pts[i];

		right_rail_pts.push_back(right_rail);
		left_rail_pts.push_back(left_rail);
		middle_rail_pts.push_back(middle_rail);
	}	
}

void Track::draw(void)
{
	glDisable(GL_LIGHTING);
	// middle rail (purple)
	glLineWidth(6.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < middle_rail_pts.size(); i++)
	{
		glColor3f(0.8, 0.0, 1.0);
		glVertex3f(middle_rail_pts[i].x(), middle_rail_pts[i].y(), middle_rail_pts[i].z());
	}
	glEnd();

	// right rail (yellow)
	glLineWidth(3.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < right_rail_pts.size(); i++)
	{
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(right_rail_pts[i].x(), right_rail_pts[i].y(), right_rail_pts[i].z());
	}
	glEnd();
	// left rail (yellow)
	glLineWidth(3.0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < left_rail_pts.size(); i++)
	{
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(left_rail_pts[i].x(), left_rail_pts[i].y(), left_rail_pts[i].z());
	}
	glEnd();
	// cross rail (purple)
	glLineWidth(3.0);
	glBegin(GL_LINES);
	for (int i = 0; i < left_rail_pts.size(); i++)
	{
		if (i % 20 == 0)
		{
			glColor3f(0.8, 0.0, 1.0);
			glVertex3f(left_rail_pts[i].x(), left_rail_pts[i].y(), left_rail_pts[i].z());
			glVertex3f(right_rail_pts[i].x(), right_rail_pts[i].y(), right_rail_pts[i].z());
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}