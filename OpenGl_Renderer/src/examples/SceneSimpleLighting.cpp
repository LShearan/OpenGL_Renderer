#pragma once
#include "SceneSimpleLighting.h"

#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"


namespace scene
{
	SimpleLigthing::SimpleLigthing()
		: m_YRotation(0.f), m_Increment(0.05f)
	{
		/* Create a perspective Camera */
		m_Camera = std::make_unique<PerspectiveCamera>(glm::vec3(-0.1f, 0.2f, 8.f), 70.f, 16.f / 9.f, 0.1f, 1000.f);

		/* Enable Blending */
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* Enable Depth Test */
		GLCALL(glEnable(GL_DEPTH_TEST));
		GLCALL(glDepthFunc(GL_LESS));

		m_Mesh = std::make_unique<Mesh>();
		m_Mesh->CreateCube(1.f, glm::vec3(0.f, 0.f, 0.f));

		m_Shader = std::make_unique<Shader>("res/shaders/SimpleLighting.shader");
		m_PointLightShader = std::make_unique<Shader>("res/shaders/PointLight.shader");
	
		m_LightPosition[0] = 1.2f;
		m_LightPosition[1] = 1.f;
		m_LightPosition[2] = 2.f;
	}

	SimpleLigthing::~SimpleLigthing()
	{
		GLCALL(glDisable(GL_BLEND));
		GLCALL(glDisable(GL_DEPTH_TEST));

		GLCALL(glBindVertexArray(0));
		GLCALL(glUseProgram(0));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	void SimpleLigthing::OnUpdate(float deltaTime, GLFWwindow* window)
	{
		m_Camera->OnUpdate(deltaTime, window);


		m_YRotation += m_Increment;
	}

	void SimpleLigthing::OnRender()
	{
		GLCALL(glClearColor(0.f, 0.f, 0.f, 1.f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


		Renderer renderer;
		// Draw the cube
		{
			glm::mat4 model = glm::rotate(glm::mat4(1.f), glm::radians(m_YRotation), glm::vec3(0.f, 1.f, 0.f));
			//glm::mat4 model = glm::mat4(1.f);
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_Model", model);
			m_Shader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
			m_Shader->SetUniformMat4f("u_Projection", m_Camera->GetProjectionMatrix());
			m_Shader->SetUniform3f("u_ObjectColour", 1.f, 0.5f, 0.31f);
			m_Shader->SetUniform3f("u_LightColour", 1.f, 1.f, 1.f);
			m_Shader->SetUniform3f("u_LightPosition", m_LightPosition[0], m_LightPosition[1], m_LightPosition[2]);
			m_Mesh->OnRender(*m_Shader);
		}

		// Draw the point light
		{
			glm::mat4 model = glm::translate(glm::mat4(1.f),glm::vec3(m_LightPosition[0], m_LightPosition[1], m_LightPosition[2]));
			model = glm::scale(model, glm::vec3(0.2f));
			glm::mat4 mvp = m_Camera->GetViewProjectionMatrix() * model;
			m_PointLightShader->Bind();
			m_PointLightShader->SetUniformMat4f("u_Model", model);
			m_PointLightShader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
			m_PointLightShader->SetUniformMat4f("u_Projection", m_Camera->GetProjectionMatrix());
			m_Mesh->OnRender(*m_PointLightShader);
		}
	}

	void SimpleLigthing::OnImGuiRender()
	{
		m_Camera->OnImGuiRender();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat("Rotation Rate: ", &m_Increment, 0.f, 1.f);
		ImGui::DragFloat3("Light Position: ", m_LightPosition,0.1,-100.f,100.f);
	}
}