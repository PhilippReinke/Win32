#ifndef TRIANGLE_SIMULATION_H
#define TRIANGLE_SIMULATION_H

#include <vector>
#include <math.h>
using namespace std;

struct TriangleSimulation
{
	// variables
	unsigned int width, height;
	unsigned int width_half, height_half;

	// model of triangle
	vector<double> triangle_vec1{ 1, 0, 0};
	vector<double> triangle_vec2{-1, 0, 0};
	vector<double> triangle_vec3{ 0, 1, 0};
	vector<vector<double>> triangle{triangle_vec1, triangle_vec2, triangle_vec3};

	// constructor
	TriangleSimulation(unsigned int w, unsigned int h)
	{
		this->width  = w; this->width_half  = (unsigned) (w/2);
		this->height = h; this->height_half = (unsigned) (h/2);
	}

	// draw vertices of triangle
	void draw_triangle(double angle_z, vector<unsigned int> &buffer_memory)
	{
		// scale, rotate, translate
		auto triangle_copy = triangle;
		for (auto &vec : triangle_copy)
		{
			// scale
			double scaling_factor = 400;
			vec[0] *= scaling_factor;
			vec[1] *= scaling_factor;
			vec[2] *= scaling_factor;

			// which rotation?
			unsigned int rot = 2;
			//
			switch (rot)
			{
				case 1:
				{
					// rotate around x axis
					double temp_y = cos(angle_z)*vec[1] - sin(angle_z)*vec[2];
					vec[2] = sin(angle_z)*vec[1] + cos(angle_z)*vec[2];
					vec[1] = temp_y;
					break;
				}
				case 2:
				{
					// rotate around y axis
					double temp_x = cos(angle_z)*vec[0] + sin(angle_z)*vec[2];
					vec[2] = -sin(angle_z)*vec[0] + cos(angle_z)*vec[2];
					vec[0] = temp_x;
					break;
				}
				case 3:
				{
					// rotate around z axis
					double temp_x = cos(angle_z)*vec[0] - sin(angle_z)*vec[1];
					vec[1] = sin(angle_z)*vec[0] + cos(angle_z)*vec[1];
					vec[0] = temp_x;
					break;
				}
			}

			// translate
			vec[2] += 1000;

			// project onto screen and rasterise
			unsigned int r = 500; // z position of screen
			vec[0] = round(r*vec[0]/vec[2]);
			vec[1] = round(r*vec[1]/vec[2]);
			vec[2] = round(r);

			// translate (0,0) is upper left corner and not center in windows world
			vec[0] += width_half;
			vec[1] += height_half;
		}

		// draw vertices
		// buffer_memory.at( this->width*triangle_copy[0][1] + triangle_copy[0][0] ) = 0xFFFFFF;
		// buffer_memory.at( this->width*triangle_copy[1][1] + triangle_copy[1][0] ) = 0xFFFFFF;
		// buffer_memory.at( this->width*triangle_copy[2][1] + triangle_copy[2][0] ) = 0xFFFFFF;

		// draw line between vertices
		// draw_line(triangle_copy[0], triangle_copy[1], buffer_memory);
		// draw_line(triangle_copy[0], triangle_copy[2], buffer_memory);
		// draw_line(triangle_copy[1], triangle_copy[2], buffer_memory);

		// fill triangle
		fill_triangle(triangle_copy[0], triangle_copy[1], triangle_copy[2], buffer_memory);
	}

	// draw line (vectors must be rasterised and on the screen)
	void draw_line(vector<double> vec1, vector<double> vec2, vector<unsigned int> &buffer)
	{
		// rectangle of possible pixels
		double max_x, min_x, max_y, min_y;
		if (vec1[0] <= vec2[0]) { max_x = vec2[0]; min_x = vec1[0]; }
		else { max_x = vec1[0]; min_x = vec2[0]; }
		if (vec1[1] <= vec2[1]) { max_y = vec2[1]; min_y = vec1[1]; }
		else { max_y = vec1[1]; min_y = vec2[1]; }
		// do not leave screen
		max_x = min((double)this->width-1, max_x); min_x = max(0.0, min_x);
		max_y = min((double)this->height-1, max_y); min_y = max(0.0, min_y);

		// values for distance
		double dif_x = vec2[0]-vec1[0];
		double dif_y = vec2[1]-vec1[1];
		double len_vec2_1 = sqrt( pow(dif_x, 2)+pow(dif_y, 2) );

		// check all possible pixel
		for (unsigned int x=min_x; x<=max_x; x++)
			for (unsigned int y=min_y; y<=max_y; y++)
			{
				// compute distance of point from line
				double distance = abs( (x-vec1[0])*dif_y-(y-vec1[1])*dif_x );
				distance /= len_vec2_1;

				// colour pixel depending on distance
				if (distance <= 0.5)
					buffer.at( this->width*y + x ) = 0xFFFFFF;
				else if (distance <= 1)
					buffer.at( this->width*y + x ) = 0xAAAAAA;
			}
	}

	// fill triangle
	void fill_triangle(vector<double> vec1, vector<double> vec2, vector<double> vec3, vector<unsigned int> &buffer)
	{
		// draw lines
		draw_line(vec1, vec2, buffer);
		draw_line(vec1, vec3, buffer);
		draw_line(vec2, vec3, buffer);

		// rectangle of possible pixels
		unsigned int max_x = max(vec1[0], max(vec2[0], vec3[0]));
		unsigned int min_x = min(vec1[0], min(vec2[0], vec3[0]));
		unsigned int max_y = max(vec1[1], max(vec2[1], vec3[1]));
		unsigned int min_y = min(vec1[1], min(vec2[1], vec3[1]));
		// do not leave screen
		max_x = min((unsigned)this->width-1, max_x); min_x = max((unsigned)0, min_x);
		max_y = min((unsigned)this->height-1, max_y); min_y = max((unsigned)0, min_y);

		// fill area between lines
		for (unsigned int x=min_x; x<=max_x; x++)
		{
			unsigned int bFoundBeginning = false, bFoundEnd = false;
			unsigned int y_begin=0, y_end=0;
			for (unsigned int y=min_y; y<max_y; y++)
			{
				if (!bFoundBeginning && buffer.at(this->width*y+x) != 0x000000 && buffer.at(this->width*(y+1)+x) == 0x000000)
				{
					y_begin = y;
					bFoundBeginning = true;
				}
				if (bFoundBeginning && buffer.at(this->width*(y+1)+x) == 0xFFFFFF)
				{
					y_end = y; bFoundEnd = true;
					break;
				}
			}
			if (bFoundEnd)
				for (unsigned int y=y_begin; y<=y_end; y++)
					buffer.at(this->width*y+x) = 0xFFFFFF;
		}
	}
};

#endif