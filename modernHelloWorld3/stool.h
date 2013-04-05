/*	

*/

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"
#include "shader.h"

class Stool : public Object
{
public:
	Stool();
	bool Initialize(int slices, int stacks, int shader);
	void legs(glm::mat4 m, int stacks);
	virtual void Draw(const glm::ivec2 & size);
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, GLfloat xRot, GLfloat yRot, int shade, const float time = 0);
	void TakeDown();
	Shader shader;
	ADSShader adsShader;
	Shader solid_color;

private:
	void BuildNormalVisualizationGeometry(int k);
	void BuildNormalVisualizationGeometry2();
	glm::vec4 colors[2];
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};
