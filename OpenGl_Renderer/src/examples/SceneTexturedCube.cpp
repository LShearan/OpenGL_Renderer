#include "SceneTexturedCube.h"

#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

namespace scene
{

	TexturedCube::TexturedCube()
		: m_YRotation(0.f), m_Increment(0.05f)
	{
		m_Camera = std::make_unique<PerspectiveCamera>(glm::vec3(-0.1f, 0.2f, 8.f), 70.f, 16.f / 9.f, 0.1f, 1000.f);

		/* Enable Blending */
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* Enable Depth Test */
		GLCALL(glEnable(GL_DEPTH_TEST));
		GLCALL(glDepthFunc(GL_LESS));

		m_Mesh = std::make_unique<Mesh>();
		m_Mesh->CreateCube(1.f, glm::vec3(0.f, 0.f, 0.f));

		m_Shader = std::make_unique<Shader>("res/shaders/TexturedCube.shader");
		m_Shader->Bind();
		m_Texture = std::make_unique<Texture>("res/textures/companioncube.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TexturedCube::~TexturedCube()
	{
		GLCALL(glDisable(GL_BLEND));
		GLCALL(glDisable(GL_DEPTH_TEST));

		GLCALL(glBindVertexArray(0));
		GLCALL(glUseProgram(0));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	void TexturedCube::OnUpdate(float deltaTime, GLFWwindow* window)
	{
		m_Camera->OnUpdate(deltaTime, window);

		m_YRotation += m_Increment;
	}

	void TexturedCube::OnRender()
	{
		GLCALL(glClearColor(0.f, 0.f, 0.f, 1.f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_Texture->Bind();

		Renderer renderer;
		{
			glm::mat4 model = glm::rotate(glm::mat4(1.f), glm::radians(m_YRotation), glm::vec3(0.f, 1.f, 0.f));
			glm::mat4 mvp = m_Camera->GetViewProjectionMatrix() * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			m_Mesh->OnRender(*m_Shader);
		}
	}

	void TexturedCube::OnImGuiRender()
	{
		m_Camera->OnImGuiRender();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat("Rotation Rate: ", &m_Increment, 0.f, 1.f);
	}

}
