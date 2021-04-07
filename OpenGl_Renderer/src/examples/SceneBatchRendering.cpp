#include "SceneBatchRendering.h"

#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

#include <array>

namespace scene
{
	BatchRenderer::BatchRenderer()
	{
		m_Camera = std::make_unique<OrthographicCamera>(-16.f, 16.f, -9.f, 9.f);
		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, sizeof(Vertex) * 1000, GL_DYNAMIC_DRAW);
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(1);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(nullptr, 1000, GL_DYNAMIC_DRAW);

		m_Shader = std::make_unique<Shader>("res/shaders/BatchQuads.shader");
		m_Shader->Bind();
		m_Texture1 = std::make_unique<Texture>("res/textures/test.png");
		//m_Texture1 = std::make_unique<Texture>("res/textures/diffuse.png");
		m_Texture2 = std::make_unique<Texture>("res/textures/brick.png");
		int samplers[2] = { 0,1 };
		m_Shader->SetUniform1iv("u_Textures", 2, samplers);
	}

	BatchRenderer::~BatchRenderer()
	{
		GLCALL(glDisable(GL_BLEND));
		GLCALL(glBindVertexArray(0));
		GLCALL(glUseProgram(0));
		GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
		GLCALL(glBindTexture(GL_TEXTURE_2D, 1));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	void BatchRenderer::OnUpdate(float deltaTime, GLFWwindow* window)
	{		
		m_Camera->OnUpdate(deltaTime, window);

		auto q0 = CreateQuad(m_Quad0Position[0], m_Quad0Position[1], 0.0f);
		auto q1 = CreateQuad(m_Quad1Position[0], m_Quad1Position[1], 1.0f);
		auto q2 = CreateQuad(m_Quad2Position[0], m_Quad2Position[1], 0.0f);

		Vertex vertices[12];
		memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
		memcpy(vertices + 4 , q1.data(), q1.size() * sizeof(Vertex));
		memcpy(vertices + 8 , q2.data(), q2.size() * sizeof(Vertex));


		/* Set dynamic vertex buffer */
		m_VertexBuffer->Bind();
		GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));


		int indices[18];

		int index = 0;
		for (int i = 0; i < 18;)
		{
			indices[i]		= index;				//0 i
			indices[i + 1]	= index + 1;			//1 i+1
			indices[i + 2]	= index + 2;			//2 i+2
			indices[i + 3]	= index + 2;			//2 i+2
			indices[i + 4]	= index + 3;			//3 i+3
			indices[i + 5]	= index;				//0 i

			i += 6;
			index += 4;
		}


		/* Set dynamic index buffer */
		m_IndexBuffer->Bind();
		GLCALL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices));



	}

	void BatchRenderer::OnRender()
	{
		GLCALL(glClearColor(0.f, 0.f, 0.f, 1.f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Texture1->Bind(0);
		m_Texture2->Bind(1);

		{
			glm::mat4 model = glm::mat4(1.f);
			glm::mat4 mvp = m_Camera->GetViewProjectionMatrix() * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

	}

	void BatchRenderer::OnImGuiRender()
	{
		m_Camera->OnImGuiRender();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::DragFloat2("Quad 1 Position ", m_Quad0Position, 0.1f);
		ImGui::DragFloat2("Quad 2 Position ", m_Quad1Position, 0.1f);
		ImGui::DragFloat2("Quad 3 Position ", m_Quad2Position, 0.1f);
	}


	std::array<Vertex, 4> BatchRenderer::CreateQuad(float x, float y, float texid)
	{
		float size = 1.f;

		Vertex v0 = { 
			{x,y,0.0f}, // Position
			{0.f,0.f},			// Tex Coords
			{texid}				// Tex ID
		};

		Vertex v1 = {
			{x + size,y, 0.f}, // Position
			{1.f,0.f},			// Tex Coords
			{texid}				// Tex ID
		};

		Vertex v2 = {
			{x + size,y + size, 0.f}, // Position
			{1.f,1.f},			// Tex Coords
			{texid}				// Tex ID
		};

		Vertex v3 = {
			{x,y + size, 0.f}, // Position
			{0.f,1.f},			// Tex Coords
			{texid}				// Tex ID
		};
		
		return { v0,v1,v2,v3 };
	}

	std::array<int, 6> BatchRenderer::CreateIndices(int startingIndex)
	{
		return { startingIndex, startingIndex + 1, startingIndex + 2,
			startingIndex + 3, startingIndex + 4, startingIndex };
	}
}