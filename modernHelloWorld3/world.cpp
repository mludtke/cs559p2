/*
	World is a grid object that can be placed into an environment 

	created:	3/20/13

*/

#include <iostream>
#include "world.h"

using namespace std;
using namespace glm;

World::World() : Object()
{
	vec4 lighter_color(MakeColor(255, 69, 0, 1.0f));
	vec4 darker_color = vec4(vec3(lighter_color) * 2.0f / 3.0f, 1.0f);
	this->colors[0] = darker_color;
	this->colors[1] = lighter_color;
	//this->colors[0] = vec4(BLUE, 1.0f);
	//this->colors[1] = vec4(BLUE, 1.0f);
}

void World::BuildNormalVisualizationGeometry()
{
	const float normal_scalar = 0.125f;
	for (GLint j = 1; j <= 3; ++j)
	{
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[this->vertices.size() - j].position));
		this->normal_vertices.push_back(VertexAttributesP(this->vertices[this->vertices.size() - j].position + this->vertices[this->vertices.size() - j].normal * normal_scalar));		
		this->normal_indices.push_back(this->normal_vertices.size() - 2);
		this->normal_indices.push_back(this->normal_vertices.size() - 1);
	}
}

bool World::Initialize(int slices)
{
	if (this->GLReturnedError("World::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;	

	//==============================================
	VertexAttributesPCN origin, yAxis, xAxis, zAxis;
	origin.position = vec3(0.0f);			
	yAxis.position  = vec3(0.0f, 1.0f, 0.0f);
	yAxis.color     = vec3(0.0f, 1.0f, 0.0f);	
	xAxis.position  = vec3(1.0f, 0.0f, 0.0f);
	xAxis.color     = vec3(1.0f, 0.0f, 0.0f);
	zAxis.position  = vec3(0.0f, 0.0f, 1.0f);
	zAxis.color     = vec3(0.0f, 0.0f, 1.0f);
	zAxis.normal    = vec3(0.0f, 0.0f, 1.0f);		
	vertices.push_back(origin);
	vertices.push_back(yAxis);
	vertices.push_back(xAxis);
	vertices.push_back(zAxis);	

	// y-axis
	vertex_indices.push_back(0);
	vertex_indices.push_back(1);	
	// x-axis
	vertex_indices.push_back(0);
	vertex_indices.push_back(2);
	// z-axis
	vertex_indices.push_back(0);
	vertex_indices.push_back(3);
	//==============================================
	
	//==============================================	
	mat4 m(1.0f);
	//m = rotate(mat4(1.0f), -90.0f, vec3(1.0f, 0.0f, 0.0f));
	
	vec3 WHITE(1.0f, 1.0f, 1.0f);

	VertexAttributesPCN top_left_corner, bot_left_corner, top_right_corner, bot_right_corner;
	top_left_corner.position  = vec3(m * vec4(-2.0f,  2.0f, 0.0f, 1.0f));
	top_left_corner.color     = WHITE;
	bot_left_corner.position  = vec3(m * vec4(-2.0f, -2.0f, 0.0f, 1.0f));
	bot_left_corner.color     = WHITE;
	top_right_corner.position = vec3(m * vec4( 2.0f,  2.0f, 0.0f, 1.0f));
	top_right_corner.color    = WHITE;
	bot_right_corner.position = vec3(m * vec4( 2.0f, -2.0f, 0.0f, 1.0f));
	bot_right_corner.color    = WHITE;
	
	for (GLfloat x = top_left_corner.position.x; x < 2.1f; x += 0.5f)
	{
		vertices.push_back(top_left_corner);
		top_left_corner.position.x += 0.5f;
		vertices.push_back(bot_left_corner);
		bot_left_corner.position.x += 0.5f;
		vertex_indices.push_back(vertices.size()-2);
		vertex_indices.push_back(vertices.size()-1);		
	}	

	//m = rotate(mat4(1.0f), -90.0f, vec3(1.0f, 0.0f, 0.0f));
	//top_left_corner.position = vec3(m * vec4(-2.0f,  2.0f, 0.0f, 1.0f));
	bot_left_corner.position = vec3(m * vec4(-2.0f, -2.0f, 0.0f, 1.0f));
	for (GLfloat y = bot_left_corner.position.y; y < 2.1f; y += 0.5f)
	{
		vertices.push_back(bot_left_corner);
		bot_left_corner.position.y += 0.5f;
		vertices.push_back(bot_right_corner);
		bot_right_corner.position.y += 0.5f;
		vertex_indices.push_back(vertices.size()-2);
		vertex_indices.push_back(vertices.size()-1);
		/*vertices.push_back(top_left_corner);
		top_left_corner.position.y -= 0.5f;
		vertices.push_back(top_right_corner);
		top_right_corner.position.y -= 0.5f;
		vertex_indices.push_back(vertices.size()-2);
		vertex_indices.push_back(vertices.size()-1);*/
	}	
	//==============================================

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

	assert(this->shader.Initialize("simple.vert", "simple.frag"));

	if (this->GLReturnedError("World::Initialize - on exit"))
		return false;

	return true;
}

void World::TakeDown()
{
	this->vertices.clear();
	this->shader.TakeDown();	
	super::TakeDown();
}

void World::Draw(const ivec2 & size)
{
	assert(false);
}

void World::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size,  GLfloat xRot, GLfloat yRot, int shade, const float time)
{
	if (this->GLReturnedError("World::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	//modelview = rotate(modelview, xrot, vec3(1.0f, 0.0f, 0.0f));
	//modelview = rotate(modelview, yrot, vec3(0.0f, 1.0f, 0.0f));

	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	shader.Use();
	shader.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
	glBindVertexArray(this->vertex_array_handle);
	glLineWidth(1.5f);
	glDrawElements(GL_LINES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glLineWidth(1.0f);
	glBindVertexArray(0);
	glUseProgram(0);	

	if (this->GLReturnedError("World::Draw - on exit"))
		return;
}
