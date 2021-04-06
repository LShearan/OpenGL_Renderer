#include "Mesh.h"

Mesh::Mesh(std::vector<VertexLayout>& vertices, std::vector<unsigned int>& indices)
{
	m_VAO = std::make_unique<VertexArray>();
	m_VertexBuffer = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(VertexLayout));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);

	m_VAO->AddBuffer(*m_VertexBuffer, layout);
	m_IndexBuffer = std::make_unique<IndexBuffer>(&indices[0], indices.size());
}

Mesh::~Mesh()
{
}

void Mesh::OnRender(Shader& shader)
{
	Renderer renderer;

	{
		renderer.Draw(*m_VAO, *m_IndexBuffer, shader);
		renderer.Clear();
	}
}