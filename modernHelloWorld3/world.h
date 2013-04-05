#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"
#include "shader.h"

class World : public Object
{
public:
	World();
	bool Initialize(int slices);
	virtual void Draw(const glm::ivec2 & size);
	virtual void Draw(const glm::mat4 & projection, glm::mat4 modelview, const glm::ivec2 & size, GLfloat xRot, GLfloat yRot, int shade, const float time = 0);
	void TakeDown();
	Shader shader;	

private:
	void BuildNormalVisualizationGeometry();	
	glm::vec4 colors[2];
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};
