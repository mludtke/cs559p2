/*	

*/

#include <iostream>
#include "cylinder.h"

using namespace std;
using namespace glm;

cylinder::cylinder() : Object()
{
	vec4 lighter_color(MakeColor(255, 69, 0, 1.0f));
	vec4 darker_color = vec4(vec3(lighter_color) /* 2.0f / 3.0f*/, 1.0f);
	this->colors[0] = lighter_color;
	this->colors[1] = lighter_color;
}


void cylinder::BuildNormalVisualizationGeometry()
{
	const float normal_scalar = 0.125f;
	for (int j = 1; j <= 3; ++j)
	{
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[this->vertices.size() - j].position));
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[this->vertices.size() - j].position + this->vertices[this->vertices.size() - j].normal * normal_scalar));
		this->normal_indices.push_back(this->normal_vertices.size() - 2);
		this->normal_indices.push_back(this->normal_vertices.size() - 1);
	}
}

bool cylinder::Initialize(int slices, int stacks, int shader)
{
//	cout << "Initialize started " << endl; 
	if (this->GLReturnedError("Top::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	if (slices < 0)
		slices = 1;

	slices *= 4;
	mat4 m;
	const vec3 n = normalize(vec3(1.0f, 1.0f, 1.0f));
	const int stacks_cylinder = stacks;
	const int slices_cylinder = slices;
	vec4 x_axis(0.1f, 0.0f, 0.0f, 1.0f);
	const float cylinder_height = 1.0f;
	const float radius = 1.0f;
	float theta = 0.0f, phi = 0.0f;
	//float x, y, z;
	vec4 location;
	vec3 color = vec3(1.0f, 0.0f, 0.0f);
	vec3 color2 = vec3(0.9f, 0.0f, 0.0f);
	const float increment_stacks = (360.0f) / float(stacks_cylinder);
	const float increment_slices = (360.0f) / slices;

	for (int i = 0; i < stacks_cylinder; ++i)
	{
		for(int j = 0; j < slices_cylinder; ++j)
		{
			VertexAttributesPCN screw_cur_vertex_top, screw_nxt_vertex_top, screw_cur_vertex_bottom, screw_nxt_vertex_bottom;

			screw_cur_vertex_top.position = vec3(m * x_axis);
			screw_cur_vertex_top.color = MAGENTA;
			screw_cur_vertex_top.normal = vec3(normalize(m * x_axis));

			screw_cur_vertex_bottom.position = vec3(m * x_axis) + vec3(0.0f, -cylinder_height/stacks_cylinder, 0.0f);
			screw_cur_vertex_bottom.color = MAGENTA;
			screw_cur_vertex_bottom.normal = vec3(normalize(vec3(m * x_axis) + vec3(0.0f, -cylinder_height/stacks_cylinder, 0.0f)));

			m = rotate(m, increment_slices, vec3(0.0f, 1.0f, 0.0f));
			
			screw_nxt_vertex_top.position = vec3(m * x_axis);
			screw_nxt_vertex_top.color = MAGENTA;
			screw_nxt_vertex_top.normal = vec3(normalize(m * x_axis));

			screw_nxt_vertex_bottom.position = vec3(m * x_axis) + vec3(0.0f, -cylinder_height/stacks_cylinder, 0.0f);
			screw_nxt_vertex_bottom.color = MAGENTA;
			screw_nxt_vertex_bottom.normal = vec3(normalize(vec3(m * x_axis) + vec3(0.0f, -cylinder_height/stacks_cylinder, 0.0f)));

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
		m = translate(m, vec3(0.0f, -cylinder_height/stacks_cylinder, 0.0f));
	}

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCN), &this->vertices[0]))
		return false;

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
	if(shader >= 1)
	{
		if (!this->shader.Initialize("gouraud_shader.vert", "gouraud_shader.frag"))
			return false;
	}
	if(shader == 3)
	{
		//if (!this->shader.Initialize("flat_shader.vert", "flat_shader.frag"))
			//return false;
		
	}
	if (!this->solid_color.Initialize("solid_shader.vert", "solid_shader.frag"))
			return false;

	if (this->GLReturnedError("Background::Initialize - on exit"))
		return false;
	return true;
}

void cylinder::TakeDown()
{
	this->vertices.clear();
	this->shader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

void cylinder::Draw(const ivec2 & size)
{
	assert(false);
}

/*	A note about drawing the normals.

	If you scale this object non-uniformly, drawing the normals will
	not be correct. This is because the normals are being stored for
	visualization as geometry. As such, scaling will not be corrected
	by the normal matrix.
*/

/*	A note about the index arrays.

	In this example, the index arrays are unsigned ints. If you know
	for certain that the number of vertices will be small enough, you
	can change the index array type to shorts or bytes. This will have
	the two fold benefit of using less storage and transferring fewer
	bytes.
*/

void cylinder::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size,GLfloat xRot, GLfloat yRot, int shade, const float time)
{
	if (this->GLReturnedError("Top::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	//modelview = rotate(modelview, xRot, vec3(1.0f, 0.0f, 0.0f));
	//modelview = rotate(modelview, yRot, vec3(0.0f, 1.0f, 0.0f));
	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	shader.Use();
	shader.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
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


