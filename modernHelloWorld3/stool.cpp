/*	
Code taken heavily from Perry Kivolowitz - UW - Madison - CS 559 demo
specifically top.cpp

Stool
*/

#include <iostream>
#include "stool.h"

using namespace std;
using namespace glm;

Stool::Stool() : Object()
{
	vec4 lighter_color(MakeColor(255, 69, 0, 1.0f));
	vec4 darker_color = vec4(vec3(lighter_color) /* 2.0f / 3.0f*/, 1.0f);
	this->colors[0] = lighter_color;
	this->colors[1] = lighter_color;
}

void Stool::BuildNormalVisualizationGeometry(int num_vertices)
{
	const float normal_scalar = 0.125f;
	for (int j = 1; j <= num_vertices; ++j)
	{
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[this->vertices.size() - j].position));
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[this->vertices.size() - j].position + this->vertices[this->vertices.size() - j].normal * normal_scalar));

		this->normal_indices.push_back(this->normal_vertices.size() - 2);
		this->normal_indices.push_back(this->normal_vertices.size() - 1);
	}
}

const vec3 leg_color(1.0f, 1.0f, 0.0f);

void Stool::legs(mat4 m, int stacks)
{
	m = rotate(m, -97.2f, vec3(1.0f, 0.0f, 0.0f));
	const vec3 n = normalize(vec3(0.5f, 0.5f, 0.0f));
	const float leg_length = 1.7f;
	const float leg_stacks = 10.0f;
	const float increment = leg_length/stacks;

	VertexAttributesPCN leg_front_top_left, leg_front_top_right, leg_front_bottom_left, leg_front_bottom_right,
						leg_back_top_left, leg_back_top_right, leg_back_bottom_left, leg_back_bottom_right;

	for(int i = 0; i < stacks; ++i)
	{

		leg_front_top_left.position = vec3(m * vec4(-0.1f, 0.0f, 0.0f, 1.0f));
		leg_front_top_left.color = leg_color;
		//leg_front_top_left.normal = vec3(-0.5f, 0.0f, 0.5f);//normalize(m * vec4(leg_front_top_left.position, 1.0f)));
		
		leg_front_top_right.position = vec3(m * vec4(0.1f, 0.0f, 0.0f, 1.0f));
		leg_front_top_right.color = leg_color;
		//leg_front_top_right.normal = vec3(0.5f, 0.0f, 0.5f);//normalize(m * vec4(leg_front_top_right.position, 1.0f)));

		leg_front_bottom_right.position = vec3(m * vec4(0.1f, increment, 0.0f, 1.0f));
		leg_front_bottom_right.color = leg_color;
		//leg_front_bottom_right.normal = vec3(0.5f, 0.0f, 0.5f);//normalize(m * vec4(leg_front_bottom_right.position, 1.0f)));

		leg_front_bottom_left.position = vec3(m * vec4(-0.1f, increment, 0.0f, 1.0f));
		leg_front_bottom_left.color = leg_color;
		//leg_front_bottom_left.normal = vec3(-0.5f, 0.0f, 0.5f);//normalize(m * vec4(leg_front_bottom_left.position, 1.0f)));

		leg_back_bottom_right.position = vec3(m * vec4(-0.1f, increment, -0.1f, 1.0f));
		leg_back_bottom_right.color = leg_color;
		//leg_back_bottom_right.normal = vec3(0.5f, 0.0f, -0.5f);//normalize(m * vec4(leg_back_bottom_right.position, 1.0f)));

		leg_back_bottom_left.position = vec3(m * vec4(0.1f, increment, -0.1f, 1.0f));
		leg_back_bottom_left.color = leg_color;
		leg_back_bottom_left.normal = vec3(-0.5f, 0.0f, -0.5f);//normalize(m * vec4(leg_back_bottom_left.position, 1.0f)));

		leg_back_top_right.position = vec3(m * vec4(0.1f, 0.0f, -0.1f, 1.0f));
		leg_back_top_right.color = leg_color;
		//leg_back_top_right.normal = vec3(0.5f, 0.0f, -0.5f);//normalize(m * vec4(leg_back_top_right.position, 1.0f)));

		leg_back_top_left.position = vec3(m * vec4(-0.1f, 0.0f, -0.1f, 1.0f));
		leg_back_top_left.color = leg_color;
		//leg_back_top_left.normal = vec3(-0.5f, 0.0f, -0.5f);//normalize(m * vec4(leg_back_top_left.position, 1.0f)));

		leg_front_top_left.normal = vec3(m * vec4((leg_front_top_left.position + leg_back_top_left.position + leg_front_top_right.position), 1.0f));
		leg_front_top_right.normal = vec3(m * vec4((leg_front_top_left.position +  leg_back_top_right.position + leg_front_top_right.position), 1.0f));
		leg_back_top_right.normal = vec3(m * vec4((leg_back_top_left.position + leg_back_top_right.position + leg_front_top_right.position), 1.0f));
		leg_back_top_left.normal = vec3(m * vec4((leg_front_top_left.position + leg_back_top_left.position + leg_back_top_right.position), 1.0f));

		leg_front_bottom_right.normal = vec3(m * vec4((leg_back_bottom_right.position + leg_front_bottom_left.position + leg_front_bottom_right.position), 1.0f));
		leg_front_bottom_left.normal = vec3(m * vec4((leg_back_bottom_left.position + leg_front_bottom_left.position + leg_front_bottom_right.position), 1.0f));
		leg_back_bottom_right.normal = vec3(m * vec4((leg_back_bottom_left.position + leg_back_bottom_right.position + leg_front_bottom_right.position), 1.0f));
		leg_back_bottom_left.normal = vec3(m * vec4((leg_back_bottom_left.position + leg_back_bottom_right.position + leg_front_bottom_left.position ), 1.0f));

		this->vertices.push_back(leg_front_bottom_left);
		this->vertices.push_back(leg_front_bottom_right);
		this->vertices.push_back(leg_front_top_left);
		this->vertices.push_back(leg_front_top_right);
		this->vertices.push_back(leg_back_bottom_left);
		this->vertices.push_back(leg_back_bottom_right);
		this->vertices.push_back(leg_back_top_left);
		this->vertices.push_back(leg_back_top_right);

		this->vertex_indices.push_back(this->vertices.size() - 8);
		this->vertex_indices.push_back(this->vertices.size() - 6);
		this->vertex_indices.push_back(this->vertices.size() - 7);
		this->vertex_indices.push_back(this->vertices.size() - 7);
		this->vertex_indices.push_back(this->vertices.size() - 6);
		this->vertex_indices.push_back(this->vertices.size() - 5);
		
		if(i == 0)	//draws the end face only for the first time
		{
			this->vertex_indices.push_back(this->vertices.size() - 6);
			this->vertex_indices.push_back(this->vertices.size() - 2);
			this->vertex_indices.push_back(this->vertices.size() - 5);

			this->vertex_indices.push_back(this->vertices.size() - 5);
			this->vertex_indices.push_back(this->vertices.size() - 2);
			this->vertex_indices.push_back(this->vertices.size() - 1);
		}

		this->vertex_indices.push_back(this->vertices.size() - 4);
		this->vertex_indices.push_back(this->vertices.size() - 5);
		this->vertex_indices.push_back(this->vertices.size() - 1);
	
		this->vertex_indices.push_back(this->vertices.size() - 4);
		this->vertex_indices.push_back(this->vertices.size() - 7);
		this->vertex_indices.push_back(this->vertices.size() - 5);
	

		this->vertex_indices.push_back(this->vertices.size() - 8);
		this->vertex_indices.push_back(this->vertices.size() - 3);
		this->vertex_indices.push_back(this->vertices.size() - 2);

		this->vertex_indices.push_back(this->vertices.size() - 8);
		this->vertex_indices.push_back(this->vertices.size() - 2);
		this->vertex_indices.push_back(this->vertices.size() - 6);



		this->vertex_indices.push_back(this->vertices.size() - 3);
		this->vertex_indices.push_back(this->vertices.size() - 4);
		this->vertex_indices.push_back(this->vertices.size() - 1);
		this->vertex_indices.push_back(this->vertices.size() - 3);
		this->vertex_indices.push_back(this->vertices.size() - 1);
		this->vertex_indices.push_back(this->vertices.size() - 2);
		
		//BuildNormalVisualizationGeometry(8);
	
		m = translate(m, vec3(0.0f, increment, 0.0f));
	}

	//draw the last end face
	this->vertices.push_back(leg_front_bottom_left);
	this->vertices.push_back(leg_front_bottom_right);
	this->vertices.push_back(leg_back_bottom_right);
	this->vertices.push_back(leg_back_bottom_left);

	this->vertex_indices.push_back(this->vertices.size() - 1);
	this->vertex_indices.push_back(this->vertices.size() - 2);
	this->vertex_indices.push_back(this->vertices.size() - 3);
	this->vertex_indices.push_back(this->vertices.size() - 2);
	this->vertex_indices.push_back(this->vertices.size() - 4);
	this->vertex_indices.push_back(this->vertices.size() - 3);
	
}

bool Stool::Initialize(int slices, int stacks, int shader)
{
	if (this->GLReturnedError("Top::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	if (slices < 0)
		slices = 1;

	slices *= 4;

	mat4 m;

	const vec3 n = normalize(vec3(1.0f, 1.0f, 1.0f));
	const vec4 x_axis_seat(0.5f, 0.0f, 0.0f, 1.0f);
	const vec4 x_axis_topDisk(0.350f, -0.2f, 0.0f, 1.0f);
	const vec4 x_axis_topDisk_top(0.325f, -0.1f, 0.0f, 1.0f);
	const vec4 x_axis_topDisk_center(0.05, -0.1f, 0.0f, 1.0f);

	const vec4 x_axis_bottomDisk(0.4f, -0.55f, 0.0f, 1.0f);
	const vec4 x_axis_bottomDisk_top(0.375f, -0.45f, 0.0f, 1.0f);
	const vec4 x_axis_bottomDisk_center(0.05f, -0.45, 0.0f, 1.0f);

	const vec3 y_axis(0.0f, 1.0f, 0.0f);
	const vec3 apex(0.0f, 0.075f, 0.0f);
	const vec3 nadir(0.0f, 0.0f, 0.0f);
	const float increment = 360.0f / float(slices);
	const vec3 neg_y(1.0f, -1.0f, 1.0f); 

	//Seat and two disks
	for (int i = 0; i < slices; ++i)
	{
		VertexAttributesPCN cur_vertex , nxt_vertex,
							top_disk_cur_vertex, top_disk_nxt_vertex, top_disk_cur_vertex_top, top_disk_nxt_vertex_top, top_disk_cur_vertex_center, top_disk_nxt_vertex_center,
							bottom_disk_cur_vertex, bottom_disk_nxt_vertex, bottom_disk_cur_vertex_top, bottom_disk_nxt_vertex_top, bottom_disk_cur_vertex_center, bottom_disk_nxt_vertex_center;
		
		//set all of the current positions
		cur_vertex.position = vec3(m * x_axis_seat);
		cur_vertex.color = vec3(0.0f, 1.0f, 0.0f);
		cur_vertex.normal = vec3(normalize(m * (x_axis_seat + vec4(0.0f, -1.0f, 0.0f, 0.0f))));

		VertexAttributesPCN cur_vertex_top(cur_vertex.position + vec3(0.0f, 0.075f, 0.0f), cur_vertex.color, vec3(normalize(m * (x_axis_seat + vec4(0.0f, 1.0f, 0.0f, 0.0f)))));

		top_disk_cur_vertex.position = vec3(m * x_axis_topDisk);
		top_disk_cur_vertex.color = vec3(0.0f, 0.0f, 1.0f);
		top_disk_cur_vertex.normal = vec3(normalize(m * (x_axis_topDisk + vec4(0.0f, -1.0f, 0.0f, 0.0f))));

		top_disk_cur_vertex_top.position = vec3(m * x_axis_topDisk_top);
		top_disk_cur_vertex_top.color = vec3(0.0f, 0.0f, 1.0f);
		top_disk_cur_vertex_top.normal = vec3(normalize(m * (x_axis_topDisk_top + vec4(0.0f, 1.0f, 0.0f, 0.0f))));

		top_disk_cur_vertex_center.position = vec3(m * x_axis_topDisk_center);
		top_disk_cur_vertex_center.color = vec3(0.0f, 0.0f, 1.0f);
		top_disk_cur_vertex_center.normal = vec3(normalize(m * -x_axis_topDisk_center + vec4(0.0f, 0.0f, 0.0f, 0.0f)));

		VertexAttributesPCN top_disk_cur_vertex_center_bottom(top_disk_cur_vertex_center.position + vec3(0.0f, -0.1f, 0.0f), top_disk_cur_vertex_center.color, vec3(normalize(m * x_axis_topDisk_center)));

		bottom_disk_cur_vertex.position = vec3(m * x_axis_bottomDisk);
		bottom_disk_cur_vertex.color = vec3(0.0f, 0.0f, 1.0f);
		bottom_disk_cur_vertex.normal = vec3(normalize(m * x_axis_bottomDisk));

		bottom_disk_cur_vertex_top.position = vec3(m * x_axis_bottomDisk_top);
		bottom_disk_cur_vertex_top.color = vec3(0.0f, 0.0f, 1.0f);
		bottom_disk_cur_vertex_top.normal = vec3(normalize(m * x_axis_bottomDisk_top));

		bottom_disk_cur_vertex_center.position = vec3(m * x_axis_bottomDisk_center);
		bottom_disk_cur_vertex_center.color = vec3(0.0f, 0.0f, 1.0f);
		bottom_disk_cur_vertex_center.normal = vec3(normalize(m * x_axis_bottomDisk_center));

		m = rotate(m, increment, y_axis);
		
		//set all of the next positions
		nxt_vertex.position = vec3(m * x_axis_seat);
		nxt_vertex.color = vec3(0.0f, 1.0f, 0.0f);
		nxt_vertex.normal = vec3(normalize(m * (x_axis_seat  + vec4(0.0f, -1.0f, 0.0f, 0.0f))));

		VertexAttributesPCN nxt_vertex_top(nxt_vertex.position + vec3(0.0f, 0.075f, 0.0f), nxt_vertex.color, vec3(normalize(m * (x_axis_seat  + vec4(0.0f, 1.0f, 0.0f, 0.0f)))));

		top_disk_nxt_vertex.position = vec3(m * x_axis_topDisk);
		top_disk_nxt_vertex.color = vec3(0.0f, 0.0f, 1.0f);
		top_disk_nxt_vertex.normal = vec3(normalize(m * (x_axis_topDisk + vec4(0.0f, -1.0f, 0.0f, 0.0f))));

		top_disk_nxt_vertex_top.position = vec3(m * x_axis_topDisk_top);
		top_disk_nxt_vertex_top.color = vec3(0.0f, 0.0f, 1.0f);
		top_disk_nxt_vertex_top.normal = vec3(normalize(m * (x_axis_topDisk_top + vec4(0.0f, 1.0f, 0.0f, 0.0f))));
		
		top_disk_nxt_vertex_center.position = vec3(m * x_axis_topDisk_center);
		top_disk_nxt_vertex_center.color = vec3(0.0f, 0.0f, 1.0f);
		top_disk_nxt_vertex_center.normal = vec3(normalize(m * -x_axis_topDisk_center + vec4(0.0f, 0.0f, 0.0, 0.0f)));

		VertexAttributesPCN top_disk_nxt_vertex_center_bottom(top_disk_nxt_vertex_center.position + vec3(0.0f, -0.1f, 0.0f), top_disk_nxt_vertex_center.color, vec3(normalize(m * x_axis_topDisk_center)));

		bottom_disk_nxt_vertex.position = vec3(m * x_axis_bottomDisk);
		bottom_disk_nxt_vertex.color = vec3(0.0f, 0.0f, 1.0f);
		bottom_disk_nxt_vertex.normal = vec3(normalize(m * x_axis_bottomDisk));

		bottom_disk_nxt_vertex_top.position = vec3(m * x_axis_bottomDisk_top);
		bottom_disk_nxt_vertex_top.color = vec3(0.0f, 0.0f, 1.0f);
		bottom_disk_nxt_vertex_top.normal = vec3(normalize(m * x_axis_bottomDisk_top));

		bottom_disk_nxt_vertex_center.position = vec3(m * x_axis_bottomDisk_center);
		bottom_disk_nxt_vertex_center.color = vec3(0.0f, 0.0f, 1.0f);
		bottom_disk_nxt_vertex_center.normal = vec3(normalize(m * x_axis_bottomDisk_center));

		//Seat
		this->vertices.push_back(VertexAttributesPCN(nadir, cur_vertex.color, vec3(0.0f, -1.0f, 0.0f)));
		this->vertices.push_back(VertexAttributesPCN(apex, cur_vertex.color, normalize(apex)));
		this->vertices.push_back(cur_vertex);
		this->vertices.push_back(nxt_vertex);
		this->vertices.push_back(cur_vertex_top);
		this->vertices.push_back(nxt_vertex_top);
		//Cylinder
		this->vertex_indices.push_back(this->vertices.size() - 4);
		this->vertex_indices.push_back(this->vertices.size() - 3);
		this->vertex_indices.push_back(this->vertices.size() - 2);
		this->vertex_indices.push_back(this->vertices.size() - 2);
		this->vertex_indices.push_back(this->vertices.size() - 3);
		this->vertex_indices.push_back(this->vertices.size() - 1);
		//top disk
		this->vertex_indices.push_back(this->vertices.size() - 2);
		this->vertex_indices.push_back(this->vertices.size() - 1);
		this->vertex_indices.push_back(this->vertices.size() - 5);
		//bottom disk
		this->vertex_indices.push_back(this->vertices.size() - 3);
		this->vertex_indices.push_back(this->vertices.size() - 4);
		this->vertex_indices.push_back(this->vertices.size() - 6);

	//	BuildNormalVisualizationGeometry(6);
		
		//Top Disk
		this->vertices.push_back(top_disk_cur_vertex_center_bottom);
		this->vertices.push_back(top_disk_nxt_vertex_center_bottom);
		this->vertices.push_back(top_disk_cur_vertex_center);
		this->vertices.push_back(top_disk_nxt_vertex_center);
		this->vertices.push_back(VertexAttributesPCN(nadir + vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), normalize(nadir + vec3(1.0f, 0.0f, 0.0f))));
		this->vertices.push_back(VertexAttributesPCN(apex + vec3(0.0f, -1.0f, 0.0f), top_disk_cur_vertex.color, normalize(apex + vec3(1.0f, 0.0f, 0.0f))));
		this->vertices.push_back(top_disk_cur_vertex);
		this->vertices.push_back(top_disk_nxt_vertex);
		this->vertices.push_back(top_disk_cur_vertex_top);
		this->vertices.push_back(top_disk_nxt_vertex_top);
		//Cylinder
		this->vertex_indices.push_back(this->vertices.size() - 4);
		this->vertex_indices.push_back(this->vertices.size() - 3);
		this->vertex_indices.push_back(this->vertices.size() - 2);
		this->vertex_indices.push_back(this->vertices.size() - 2);
		this->vertex_indices.push_back(this->vertices.size() - 3);
		this->vertex_indices.push_back(this->vertices.size() - 1);
		//top disk
		this->vertex_indices.push_back(this->vertices.size() - 2);
		this->vertex_indices.push_back(this->vertices.size() - 1);
		this->vertex_indices.push_back(this->vertices.size() - 8);
		this->vertex_indices.push_back(this->vertices.size() - 8);
		this->vertex_indices.push_back(this->vertices.size() - 1);
		this->vertex_indices.push_back(this->vertices.size() - 7);
		//bottom disk
		this->vertex_indices.push_back(this->vertices.size() - 3);
		this->vertex_indices.push_back(this->vertices.size() - 4);
		this->vertex_indices.push_back(this->vertices.size() - 9);
		this->vertex_indices.push_back(this->vertices.size() - 9);
		this->vertex_indices.push_back(this->vertices.size() - 4);
		this->vertex_indices.push_back(this->vertices.size() - 10);
		//innercylinder
		this->vertex_indices.push_back(this->vertices.size() - 8);
		this->vertex_indices.push_back(this->vertices.size() - 7);
		this->vertex_indices.push_back(this->vertices.size() - 9);
		this->vertex_indices.push_back(this->vertices.size() - 9);
		this->vertex_indices.push_back(this->vertices.size() - 10);
		this->vertex_indices.push_back(this->vertices.size() - 8);

	//	BuildNormalVisualizationGeometry(10);
	
		//BuildNormalVisualizationGeometry(9);

		//Bottom Disk
		this->vertices.push_back(VertexAttributesPCN(bottom_disk_cur_vertex_center.position + vec3(0.0f, -0.1f, 0.0f), bottom_disk_cur_vertex_center.color, top_disk_cur_vertex_center.normal));
		this->vertices.push_back(VertexAttributesPCN(bottom_disk_nxt_vertex_center.position + vec3(0.0f, -0.1f, 0.0f), bottom_disk_nxt_vertex_center.color, top_disk_nxt_vertex_center.normal));
		this->vertices.push_back(bottom_disk_cur_vertex_center);
		this->vertices.push_back(bottom_disk_nxt_vertex_center);
		this->vertices.push_back(VertexAttributesPCN(nadir + vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), normalize(nadir + vec3(0.0f, -1.0f, 0.0f))));
		this->vertices.push_back(VertexAttributesPCN(apex + vec3(0.0f, -1.0f, 0.0f), bottom_disk_cur_vertex.color, normalize(apex + vec3(0.0f, -1.0f, 0.0f))));
		this->vertices.push_back(bottom_disk_cur_vertex);
		this->vertices.push_back(bottom_disk_nxt_vertex);
		this->vertices.push_back(bottom_disk_cur_vertex_top);
		this->vertices.push_back(bottom_disk_nxt_vertex_top);
		//Cylinder
		this->vertex_indices.push_back(this->vertices.size() - 4);
		this->vertex_indices.push_back(this->vertices.size() - 3);
		this->vertex_indices.push_back(this->vertices.size() - 2);
		this->vertex_indices.push_back(this->vertices.size() - 2);
		this->vertex_indices.push_back(this->vertices.size() - 3);
		this->vertex_indices.push_back(this->vertices.size() - 1);
		//top disk
		this->vertex_indices.push_back(this->vertices.size() - 2);
		this->vertex_indices.push_back(this->vertices.size() - 1);
		this->vertex_indices.push_back(this->vertices.size() - 8);
		this->vertex_indices.push_back(this->vertices.size() - 8);
		this->vertex_indices.push_back(this->vertices.size() - 1);
		this->vertex_indices.push_back(this->vertices.size() - 7);
		//bottom disk
		this->vertex_indices.push_back(this->vertices.size() - 3);
		this->vertex_indices.push_back(this->vertices.size() - 4);
		this->vertex_indices.push_back(this->vertices.size() - 9);
		this->vertex_indices.push_back(this->vertices.size() - 9);
		this->vertex_indices.push_back(this->vertices.size() - 4);
		this->vertex_indices.push_back(this->vertices.size() - 10);
		//innercylinder
		this->vertex_indices.push_back(this->vertices.size() - 8);
		this->vertex_indices.push_back(this->vertices.size() - 7);
		this->vertex_indices.push_back(this->vertices.size() - 9);
		this->vertex_indices.push_back(this->vertices.size() - 9);
		this->vertex_indices.push_back(this->vertices.size() - 10);
		this->vertex_indices.push_back(this->vertices.size() - 8);

	//	BuildNormalVisualizationGeometry(10);
	}

	const float stacks_screw = 12.0f;
	const vec4 screw_x_axis(0.05f, 0.0f, 0.0f, 1.0f);
	const float screw_height = 1.0f;
	float screw_slices = slices/4.0f;
	if (screw_slices < 4)
		screw_slices = 4;
	const float screw_increment = 360.0f / screw_slices;
	//Screw
	for (int i = 0; i < screw_slices; ++i)
	{
		VertexAttributesPCN screw_cur_vertex_top, screw_nxt_vertex_top;

		screw_cur_vertex_top.position = vec3(m * screw_x_axis);
		screw_cur_vertex_top.color = vec3(1.0f, 0.0f, 0.0f);
		screw_cur_vertex_top.normal = vec3(m * vec4(n, 1.0));
		
		m = rotate(m, screw_increment, vec3(0.0f, 1.0f, 0.0f));

		screw_nxt_vertex_top.position = vec3(m * screw_x_axis);
		screw_nxt_vertex_top.color = vec3(1.0f, 0.0f, 0.0f);
		screw_nxt_vertex_top.normal = vec3(m * vec4(n, 1.0f));

		this->vertices.push_back(screw_cur_vertex_top);
		this->vertices.push_back(screw_nxt_vertex_top);
		this->vertices.push_back(VertexAttributesPCN(nadir, screw_cur_vertex_top.color, normalize(apex)));

		this->vertex_indices.push_back(vertices.size() - 3);
		this->vertex_indices.push_back(vertices.size() - 2);
		this->vertex_indices.push_back(vertices.size() - 1);
	}

	for (int i = 0; i < stacks_screw; ++i)
	{
		for(int j = 0; j < screw_slices; ++j)
		{
			VertexAttributesPCN screw_cur_vertex_top, screw_nxt_vertex_top, screw_cur_vertex_bottom, screw_nxt_vertex_bottom;

			screw_cur_vertex_top.position = vec3(m * screw_x_axis);
			screw_cur_vertex_top.color = vec3(1.0f, 0.0f, 0.0f);
			screw_cur_vertex_top.normal = vec3(normalize(m * screw_x_axis));

			screw_cur_vertex_bottom.position = vec3(m * screw_x_axis) + vec3(0.0f, -screw_height/stacks_screw, 0.0f);
			screw_cur_vertex_bottom.color = vec3(1.0f, 0.0f, 0.0f);
			screw_cur_vertex_bottom.normal = vec3(normalize(vec3(m * screw_x_axis) + vec3(0.0f, -screw_height/stacks_screw, 0.0f)));

			m = rotate(m, screw_increment, vec3(0.0f, 1.0f, 0.0f));
			
			screw_nxt_vertex_top.position = vec3(m * screw_x_axis);
			screw_nxt_vertex_top.color = vec3(1.0f, 0.0f, 0.0f);
			screw_nxt_vertex_top.normal = vec3(normalize(m * screw_x_axis));

			screw_nxt_vertex_bottom.position = vec3(m * screw_x_axis) + vec3(0.0f, -screw_height/stacks_screw, 0.0f);
			screw_nxt_vertex_bottom.color = vec3(1.0f, 0.0f, 0.0f);
			screw_nxt_vertex_bottom.normal = vec3(normalize(vec3(m * screw_x_axis) + vec3(0.0f, -screw_height/stacks_screw, 0.0f)));

			this->vertices.push_back(screw_cur_vertex_top);
			this->vertices.push_back(screw_cur_vertex_bottom);
			this->vertices.push_back(screw_nxt_vertex_top);
			this->vertices.push_back(screw_nxt_vertex_bottom);
			

			this->vertex_indices.push_back(this->vertices.size() - 1);
			this->vertex_indices.push_back(this->vertices.size() - 4);
			this->vertex_indices.push_back(this->vertices.size() - 3);

			this->vertex_indices.push_back(this->vertices.size() - 2);
			this->vertex_indices.push_back(this->vertices.size() - 4);
			this->vertex_indices.push_back(this->vertices.size() - 1);
			//BuildNormalVisualizationGeometry(4);
		}
		m = translate(m, vec3(0.0f, -screw_height/stacks_screw, 0.0f));
	}

	for (int i = 0; i < screw_slices; ++i)
	{
		VertexAttributesPCN screw_cur_vertex_top, screw_nxt_vertex_top;

		screw_cur_vertex_top.position = vec3(m * screw_x_axis);
		screw_cur_vertex_top.color = vec3(1.0f, 0.0f, 0.0f);
		screw_cur_vertex_top.normal = vec3(m * vec4(n, 1.0));
		
		m = rotate(m, screw_increment, vec3(0.0f, 1.0f, 0.0f));

		screw_nxt_vertex_top.position = vec3(m * screw_x_axis);
		screw_nxt_vertex_top.color = vec3(1.0f, 0.0f, 0.0f);
		screw_nxt_vertex_top.normal = vec3(m * vec4(n, 1.0f));

		this->vertices.push_back(screw_cur_vertex_top);
		this->vertices.push_back(screw_nxt_vertex_top);
		this->vertices.push_back(VertexAttributesPCN(nadir + vec3(0.0f, -screw_height, 0.0f), screw_cur_vertex_top.color, normalize(apex)));

		this->vertex_indices.push_back(vertices.size() - 2);
		this->vertex_indices.push_back(vertices.size() - 3);
		this->vertex_indices.push_back(vertices.size() - 1);

		//BuildNormalVisualizationGeometry(3);
	}


	m = rotate(m, 90.0f, vec3(1.0f, 0.0f, 0.0f));
	m = translate(m, vec3(0.0f, 0.0f, 0.3f));

	const float stacks_torus = 12.0f;
	const float minor_radius_torus = 0.05f;
	const float major_radius_torus = 0.45f;
	vec4 x_location_torus;
	vec4 y_location_torus;
	vec4 z_location_torus;
	vec4 torus_location;
	float x, y, z;
	float theta = 0.0f, phi = 0.0f;
	const float increment_stacks = (2.0f * 3.14159f) / float(stacks_torus);
	const float increment_slices = (2.0f * 3.14159f) / slices;

	//Torus
	for (int i = 0; i < slices; ++i)
	{
		for (int j = 0; j < stacks_torus; ++j)
		{
			VertexAttributesPCN torus_cur_vertex, torus_nxt_vertex, torus_nxt2_vertex, torus_cur2_vertex;

			x = (major_radius_torus + minor_radius_torus * cos(phi)) * cos(theta);
			y = (major_radius_torus + minor_radius_torus * cos(phi)) * sin(theta);
			z = minor_radius_torus * sin(phi);
			torus_location = vec4(x, y, z, 1.0f);

			torus_cur_vertex.position = vec3(m * torus_location);
			torus_cur_vertex.normal = vec3(m * torus_location);
			torus_cur_vertex.color = vec3(1.0f, 0.0f, 1.0f);

			theta += increment_slices;

			x = (major_radius_torus + minor_radius_torus * cos(phi)) * cos(theta);
			y = (major_radius_torus + minor_radius_torus * cos(phi)) * sin(theta);
			z = minor_radius_torus * sin(phi);
			torus_location = vec4(x, y, z, 1.0f);

			torus_cur2_vertex.position = vec3(m * torus_location);
			torus_cur2_vertex.normal = vec3(m * torus_location);
			torus_cur2_vertex.color = vec3(1.0f, 0.0f, 1.0f);

			phi += increment_stacks;

			x = (major_radius_torus + minor_radius_torus * cos(phi)) * cos(theta);
			y = (major_radius_torus + minor_radius_torus * cos(phi)) * sin(theta);
			z = minor_radius_torus * sin(phi);
			torus_location = vec4(x, y, z, 1.0f);

			torus_nxt2_vertex.position = vec3(m * torus_location);
			torus_nxt2_vertex.normal = vec3(m * torus_location);
			torus_nxt2_vertex.color = vec3(1.0f, 0.0f, 1.0f);

			theta -= increment_slices;

			x = (major_radius_torus + minor_radius_torus * cos(phi)) * cos(theta);
			y = (major_radius_torus + minor_radius_torus * cos(phi)) * sin(theta);
			z = minor_radius_torus * sin(phi);
			torus_location = vec4(x, y, z, 1.0f);

			torus_nxt_vertex.position = vec3(m * torus_location);
			torus_nxt_vertex.normal = vec3(m * torus_location);
			torus_nxt_vertex.color = vec3(1.0f, 0.0f, 1.0f);

			this->vertices.push_back(torus_cur2_vertex);
			this->vertices.push_back(torus_nxt2_vertex);
			this->vertices.push_back(torus_nxt_vertex);
			this->vertices.push_back(torus_cur_vertex);

			this->vertex_indices.push_back(this->vertices.size() - 4);
			this->vertex_indices.push_back(this->vertices.size() - 2);
			this->vertex_indices.push_back(this->vertices.size() - 3);

			this->vertex_indices.push_back(this->vertices.size() - 4);
			this->vertex_indices.push_back(this->vertices.size() - 1);
			this->vertex_indices.push_back(this->vertices.size() - 2);

			//BuildNormalVisualizationGeometry(4);
		}

		theta += increment_slices;
	}
	m = translate(m, vec3(0.0f, 0.65f, 0.48f));
	Stool::legs(m, stacks);
	m = translate(m, vec3(-0.65f, -0.65f, 0.0f));
	m = rotate(m, 90.0f, vec3(0.0f, 0.0f, 1.0f));
	Stool::legs(m, stacks);
	m = translate(m, vec3(-0.65f, -0.65f, 0.0f));
	m = rotate(m, 90.0f, vec3(0.0f, 0.0f, 1.0f));
	Stool::legs(m, stacks);
	m = translate(m, vec3(-0.65f, -0.65f, 0.0f));
	m = rotate(m, 90.0f, vec3(0.0f, 0.0f, 1.0f));
	Stool::legs(m, stacks);
	

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCN), &this->vertices[0]))
		return false;

	/*	The VertexAttributesPCN class stores vertex attributes: position, color and normal in that order.

		Vertex attributes are stored in an interleaved manner aiding speed of vertex processing.
	*/

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) * 2));	// Note offset - legacy of older code
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) * 1));	// Same
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (this->normal_vertices.size() > 0)
	{
		if (!this->PostGLInitialize(&this->normal_array_handle, &this->normal_coordinate_handle, this->normal_vertices.size() * sizeof(VertexAttributesP), &this->normal_vertices[0]))
			return false;

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesP), (GLvoid *) 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	if(shader == 0)
	{
		if (!this->shader.Initialize("phong_shader.vert", "phong_shader.frag"))
			return false;
	}
	if(shader == 1)
	{
		if (!this->shader.Initialize("gouraud_shader.vert", "gouraud_shader.frag"))
			return false;
	}
	if(shader == 2)
	{
		if(!this->adsShader.Initialize("light.vert", "light.frag"))
			return false;
		//if (!this->shader.Initialize("flat_shader.vert", "flat_shader.frag"))
			//return false;
	}
	if(shader == 3)
	{
		if (!this->adsShader.Initialize("flat_shader.vert", "flat_shader.frag"))
			return false;
	}

	if (!this->solid_color.Initialize("solid_shader.vert", "solid_shader.frag"))
		return false;
	if (this->GLReturnedError("Background::Initialize - on exit"))
		return false;

	return true;
}

void Stool::TakeDown()
{
	this->vertices.clear();
	this->shader.TakeDown();
	this->adsShader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

void Stool::Draw(const ivec2 & size)
{
	assert(false);
}


void Stool::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, GLfloat xRot, GLfloat yRot, int shade, const float time)
{
	if (this->GLReturnedError("Top::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	//modelview = rotate(modelview, xRot, vec3(1.0f, 0.0f, 0.0f));
	//modelview = rotate(modelview, yRot, vec3(0.0f, 1.0f, 0.0f));
	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	if(shade < 2)
	{
		shader.Use();
		shader.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
	}
	if(shade == 2)
	{
		adsShader.Use();
		adsShader.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
		adsShader.SetLight(glm::vec4(0.0f, 0.0f, 2.0f, 1.0f), glm::vec3(0.2f), glm::vec3(0.7f), vec3(0.7f));
		adsShader.SetMaterial(vec3(0.24725f, 0.21995f, 0.0745f), vec3(0.75164f, 0.60648f, 0.22648f), vec3(0.628281f, 0.555802f, 0.366065f), 51.2f);
	}
	if(shade == 3)
	{
		adsShader.Use();
		adsShader.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
		adsShader.SetLight(glm::vec4(0.0f, 0.0f, 2.0f, 1.0f), glm::vec3(0.2f), glm::vec3(0.7f), vec3(0.7f));
		adsShader.SetMaterial(vec3(0.24725f, 0.21995f, 0.0745f), vec3(0.75164f, 0.60648f, 0.22648f), vec3(0.628281f, 0.555802f, 0.366065f), 51.2f);
	}
	glBindVertexArray(this->vertex_array_handle);
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);
	glUseProgram(0);

	if (this->draw_normals)
	{
		this->solid_color.Use();
		this->solid_color.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
		glBindVertexArray(this->normal_array_handle);
		glDrawElements(GL_LINES , this->normal_indices.size(), GL_UNSIGNED_INT , &this->normal_indices[0]);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	if (this->GLReturnedError("Top::Draw - on exit"))
		return;
}



