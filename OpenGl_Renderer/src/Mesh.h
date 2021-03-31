#pragma once

#include "glm/glm.hpp"

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
struct TextureLayout
{
	unsigned int ID;
	std::string Type;
	std::string Path;
};

class Mesh
{
private:

	unsigned int m_RendererID;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::vector<std::unique_ptr<Texture>> m_Textures;

	std::vector<VertexLayout> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<TextureLayout> m_TextureLayouts;


public:
	Mesh(const std::vector<VertexLayout> vertices, std::vector<unsigned int> indices, std::vector<TextureLayout> textures)
		:m_Vertices(vertices), m_Indices(indices), m_TextureLayouts(textures)
	{
		SetupMesh();
	}

	void OnRender(Shader& shader)
	{
		for (unsigned int i = 0; i < m_Textures.size(); i++)
		{
			m_Textures[i].get()->Bind(i);
		}
		/* Bind the vertex array and index buffer and shader*/
		m_VAO.get()->Bind();
		m_IndexBuffer.get()->Bind();
		shader.Bind();

		GLCALL(glDrawElements(GL_TRIANGLES, m_IndexBuffer.get()->GetCount(), GL_UNSIGNED_INT, nullptr));

		m_VAO.get()->Unbind();
		m_IndexBuffer.get()->Unbind();
		shader.Unbind();
		
		for (unsigned int i = 0; i < m_Textures.size(); i++)
		{
			m_Textures[i].get()->Unbind();
		}
	}
	
private:
	void SetupMesh()
	{
		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(&m_Vertices[0],m_Vertices.size() * sizeof(VertexLayout));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(&m_Indices[0], m_Indices.size());

		for (int i = 0; i < m_TextureLayouts.size(); i++)
		{
			if(m_TextureLayouts[i].Type == "u_Diffuse")
				m_Textures.push_back(std::make_unique<Texture>(m_TextureLayouts[i].Path));
		}
	}
};