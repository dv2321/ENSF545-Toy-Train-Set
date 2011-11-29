#ifndef TRACK_H
#define TRACK_H

#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "Point.h"
using namespace std;

class Track
{
public:
	Track();
	~Track();

	vector<Point> pts;	
	
	bool find_arc_segment(float distance_travelled, int &segment_index);
	float calculate_slope(int segment_index);
	float calculate_ds(float distance_travelled, int segment_index);		// calculate position on arc segment
	void draw(void);

	float total_length;

private:
	vector<Point> ctrl_pts;
	vector<Point> left_rail_pts, right_rail_pts, middle_rail_pts, cross_rail_pts;

	vector<float> inc_arc_length_segments;			// incremental arc length of track for each point

	void cubic_mask(vector<Point> &fine_points, vector<Point> &coarse_points);
	void cubic_BSpline(vector<Point> &fine_points, vector<Point> &control_points, int num_steps);

	void import_track_pts(void);
	void build_track(void);
};

#endif
