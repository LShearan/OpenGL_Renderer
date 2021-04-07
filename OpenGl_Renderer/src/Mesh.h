#pragma once

#include "glm/glm.hpp"

#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <string>
#include <vector>
#include <memory>


struct VertexLayout
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};


class Mesh
{
private:

	unsigned int m_RendererID;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;



public:

	Mesh(std::vector<VertexLayout>& vertices, std::vector<unsigned int>& indices);
	~Mesh();

	void OnRender(Shader& shader);
	
private:

};