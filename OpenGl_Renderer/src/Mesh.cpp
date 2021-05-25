#include "Mesh.h"

Mesh::Mesh()
	: m_Initialised(false)
{}

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

	m_Initialised = true;
}

Mesh::~Mesh()
{
}

void Mesh::OnRender(Shader& shader)
{
	Renderer renderer;

	{
		renderer.Draw(*m_VAO, *m_IndexBuffer, shader);
	}
}

void Mesh::CreateCube(float size, glm::vec3 center)
{
	/*glm::vec3 frontTopRight		= center + glm::vec3(0.5f, 0.5f, -0.5f);
	glm::vec3 backTopRight		= center + glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 frontTopLeft		= center + glm::vec3(-0.5f, 0.5f, -0.5f);
	glm::vec3 backTopLeft		= center + glm::vec3(-0.5f, 0.5f, 0.5f);
	glm::vec3 frontBottomRight	= center + glm::vec3(0.5f, -0.5f, -0.5f);
	glm::vec3 backBottomRight	= center + glm::vec3(0.5f, -0.5f, 0.5f);
	glm::vec3 frontBottomLeft	= center + glm::vec3(-0.5f, -0.5f, -0.5f);
	glm::vec3 backBottomLeft	= center + glm::vec3(-0.5f, -0.5f, 0.5f);*/

	float factor = size / 2.f;
	glm::vec3 frontTopRight		= center + glm::vec3(factor, factor, -factor);
	glm::vec3 backTopRight		= center + glm::vec3(factor, factor, factor);
	glm::vec3 frontTopLeft		= center + glm::vec3(-factor, factor, -factor);
	glm::vec3 backTopLeft		= center + glm::vec3(-factor, factor, factor);
	glm::vec3 frontBottomRight	= center + glm::vec3(factor, -factor, -factor);
	glm::vec3 backBottomRight	= center + glm::vec3(factor, -factor, factor);
	glm::vec3 frontBottomLeft	= center + glm::vec3(-factor, -factor, -factor);
	glm::vec3 backBottomLeft	= center + glm::vec3(-factor, -factor, factor);

	glm::vec3 topNormal = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 frontNormal = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 backNormal = glm::vec3(0.f, 0.f, 1.f);
	glm::vec3 leftNormal = glm::vec3(-1.f, 0.f, 0.f);
	glm::vec3 rightNormal = glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 bottomNormal = glm::vec3(0.f, -1.f, 0.f);

	std::vector<VertexLayout> verts;
	verts.reserve(24);

	// Top
	// BackTopRight, BackTopLeft, FrontTopLeft, FrontTopRight
	verts.push_back(VertexLayout(backTopRight,		topNormal, glm::vec2(0.f, 0.f)));
	verts.push_back(VertexLayout(backTopLeft,		topNormal, glm::vec2(1.f, 0.f)));
	verts.push_back(VertexLayout(frontTopLeft,		topNormal, glm::vec2(1.f, 1.f)));
	verts.push_back(VertexLayout(frontTopRight,		topNormal, glm::vec2(0.f, 1.f)));
	// Front
	// FrontTopLeft, FrontTopRight, FrontBottomLeft, FrontBottomRight
	verts.push_back(VertexLayout(frontTopLeft,		frontNormal, glm::vec2(0.f, 1.f)));
	verts.push_back(VertexLayout(frontTopRight,		frontNormal, glm::vec2(1.f, 1.f)));
	verts.push_back(VertexLayout(frontBottomLeft,	frontNormal, glm::vec2(0.f, 0.f)));
	verts.push_back(VertexLayout(frontBottomRight,	frontNormal, glm::vec2(1.f, 0.f)));
	// Bottom
	// BackBottomRight, BackBottomLeft, FrontBottomLeft, FrontBottomRight
	verts.push_back(VertexLayout(backBottomRight,	bottomNormal, glm::vec2(0.f, 0.f)));
	verts.push_back(VertexLayout(backBottomLeft,	bottomNormal, glm::vec2(1.f, 0.f)));
	verts.push_back(VertexLayout(frontBottomLeft,	bottomNormal, glm::vec2(1.f, 1.f)));
	verts.push_back(VertexLayout(frontBottomRight,	bottomNormal, glm::vec2(0.f, 1.f)));
	// Back
	// BackTopRight, BackTopLeft, BackBottomRight, BackBottomLeft
	verts.push_back(VertexLayout(backTopRight,		backNormal, glm::vec2(0.f, 1.f)));
	verts.push_back(VertexLayout(backTopLeft,		backNormal, glm::vec2(1.f, 1.f)));
	verts.push_back(VertexLayout(backBottomRight,	backNormal, glm::vec2(0.f, 0.f)));
	verts.push_back(VertexLayout(backBottomLeft,	backNormal, glm::vec2(1.f, 0.f)));
	// Right
	// BackTopRight, FrontTopRight, BackBottomRight, FrontBottomRight
	verts.push_back(VertexLayout(backTopRight,		rightNormal, glm::vec2(1.f, 1.f)));
	verts.push_back(VertexLayout(frontTopRight,		rightNormal, glm::vec2(0.f, 1.f)));
	verts.push_back(VertexLayout(backBottomRight,	rightNormal, glm::vec2(1.f, 0.f)));
	verts.push_back(VertexLayout(frontBottomRight,	rightNormal, glm::vec2(0.f, 0.f)));
	//Left
	// BackTopLeft, FrontTopLeft, BackBottomLeft, FrontBottomLeft
	verts.push_back(VertexLayout(backTopLeft,		leftNormal, glm::vec2(0.f, 1.f)));
	verts.push_back(VertexLayout(frontTopLeft,		leftNormal, glm::vec2(1.f, 1.f)));
	verts.push_back(VertexLayout(backBottomLeft,	leftNormal, glm::vec2(0.f, 0.f)));
	verts.push_back(VertexLayout(frontBottomLeft,	leftNormal, glm::vec2(1.f, 0.f)));

	unsigned int indices[] =
	{
		0, 1, 3, //top 1 
		3, 1, 2, //top 2 
		4, 6, 7, //front 1 
		7, 5, 4, //front 2 
		11, 10, 9, //bottom 1 
		9, 8, 11, //bottom 2 
		15, 13, 14, //back 1
		14, 13, 12, //back 2 
		18, 17, 19, //right 1 
		17, 18, 16, //right 2 
		22, 23, 21, //left 1 
		22, 20, 21  //left 2
	};

	m_VAO = std::make_unique<VertexArray>();
	m_VertexBuffer = std::make_unique<VertexBuffer>(&verts[0], verts.size() * sizeof(VertexLayout));
	VertexBufferLayout layout;
	layout.Push<float>(3); //Position
	layout.Push<float>(3); //Normal
	layout.Push<float>(2); //TexCoords

	m_VAO->AddBuffer(*m_VertexBuffer, layout);
	m_IndexBuffer = std::make_unique<IndexBuffer>(&indices[0], 3 * 12);

	m_Initialised = true;

}

void Mesh::CreateQuad(float size, glm::vec2 center)
{
}

void Mesh::CreateMeshFromFile(const std::string filePath)
{
}
