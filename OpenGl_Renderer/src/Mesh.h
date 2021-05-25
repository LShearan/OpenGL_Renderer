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

	VertexLayout(glm::vec3 pos,glm::vec3 norm, glm::vec2 tex)
	{
		Position = pos;
		Normal = norm;
		TexCoords = tex;
	}
};


class Mesh
{
private:

	unsigned int m_RendererID;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;

	bool m_Initialised;

public:

	Mesh();
	Mesh(std::vector<VertexLayout>& vertices, std::vector<unsigned int>& indices);
	~Mesh();

	void OnRender(Shader& shader);

	void CreateCube(float size, glm::vec3 center);
	void CreateQuad(float size, glm::vec2 center);
	void CreateMeshFromFile(const std::string filePath);
	
private:

};