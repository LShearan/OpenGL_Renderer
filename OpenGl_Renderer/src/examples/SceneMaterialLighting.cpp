#pragma once
#include "SceneMaterialLighting.h"

#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"


namespace scene
{
	MaterialLighting::MaterialLighting()
		: m_YRotation(0.f), m_Increment(0.05f)
	{

		float cubePosition[] =
		{
			// Top
			 1.f,  1.f,  1.f,	 0.f,1.f,0.f,	0.f,0.f,	// 0
			-1.f,  1.f,  1.f,	 0.f,1.f,0.f,	1.f,0.f,	// 1
			-1.f,  1.f, -1.f,	 0.f,1.f,0.f,	1.f,1.f,	// 2
			 1.f,  1.f, -1.f,	 0.f,1.f,0.f,	0.f,1.f,	// 3
			// Front					   	
			-1.f,  1.f, -1.f,	 0.f,0.f,-1.f,	0.f,1.f,	// 4 
			 1.f,  1.f, -1.f,	 0.f,0.f,-1.f,	1.f,1.f,	// 5 
			-1.f, -1.f, -1.f,	 0.f,0.f,-1.f,	0.f,0.f,	// 6 
			 1.f, -1.f, -1.f,	 0.f,0.f,-1.f,	1.f,0.f,	// 7 
			 // Bottom					  	
			  1.f, -1.f,  1.f,	 0.f,-1.f,0.f,	0.f,0.f,	// 8
			 -1.f, -1.f,  1.f,	 0.f,-1.f,0.f,	1.f,0.f,	// 9
			 -1.f, -1.f, -1.f,	 0.f,-1.f,0.f,	1.f,1.f,	// 10
			  1.f, -1.f, -1.f,	 0.f,-1.f,0.f,	0.f,1.f,	// 11
			 // Back					  	
			  1.f,  1.f,  1.f,	 0.f,0.f,1.f,	0.f,1.f,	// 12
			 -1.f,  1.f,  1.f,	 0.f,0.f,1.f,	1.f,1.f,	// 13
			  1.f, -1.f,  1.f,	 0.f,0.f,1.f,	0.f,0.f,	// 14
			 -1.f, -1.f,  1.f,	 0.f,0.f,1.f,	1.f,0.f,	// 15
			 // Right					   	
			  1.f,  1.f,  1.f,	 1.f,0.f,0.f,	1.f,1.f,	// 16
			  1.f,  1.f, -1.f,	 1.f,0.f,0.f,	0.f,1.f,	// 17
			  1.f, -1.f,  1.f,	 1.f,0.f,0.f,	1.f,0.f,	// 18
			  1.f, -1.f, -1.f,	 1.f,0.f,0.f,	0.f,0.f,	// 19
			 //Left						   
			 -1.f,  1.f,  1.f,	 -1.f,0.f,0.f,	0.f,1.f,	// 20
			 -1.f,  1.f, -1.f,	 -1.f,0.f,0.f,	1.f,1.f,	// 21
			 -1.f, -1.f,  1.f,	 -1.f,0.f,0.f,	0.f,0.f,	// 22
			 -1.f, -1.f, -1.f,	 -1.f,0.f,0.f,	1.f,0.f,	// 23
		};
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

		/* Create a perspective Camera */
		m_Camera = std::make_unique<PerspectiveCamera>(glm::vec3(-0.1f, 0.2f, 8.f), 70.f, 16.f / 9.f, 0.1f, 1000.f);

		/* Enable Blending */
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* Enable Depth Test */
		GLCALL(glEnable(GL_DEPTH_TEST));
		GLCALL(glDepthFunc(GL_LESS));
		
		/* Setup light properties */
		m_LightPosition[0] = 1.2f;
		m_LightPosition[1] = 1.f;
		m_LightPosition[2] = 2.f;
		m_LightAmbient[0] = 0.2f;
		m_LightAmbient[1] = 0.2f;
		m_LightAmbient[2] = 0.2f;
		m_LightDiffuse[0] = 0.5f;
		m_LightDiffuse[1] = 0.5f;
		m_LightDiffuse[2] = 0.5f;
		m_LightSpecular[0] = 1.f;
		m_LightSpecular[1] = 1.f;
		m_LightSpecular[2] = 1.f;

		/* Setup material properties */
		m_MaterialShininess = 32.f;

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(cubePosition, 8 * 24 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * 12);

		m_Shader = std::make_unique<Shader>("res/shaders/MaterialLighting.shader");

		m_Texture1 = std::make_unique<Texture>("res/textures/cratediffuse.png");
		m_Texture2 = std::make_unique<Texture>("res/textures/cratespecular.png");
		m_Shader->Bind();
		m_Shader->SetUniform1i("u_Material.diffuse", 0);
		m_Shader->SetUniform1i("u_Material.specular", 1);
		m_Shader->Unbind();


		m_PointLightShader = std::make_unique<Shader>("res/shaders/PointLight.shader");
	}

	MaterialLighting::~MaterialLighting()
	{
		GLCALL(glDisable(GL_BLEND));
		GLCALL(glDisable(GL_DEPTH_TEST));

		GLCALL(glBindVertexArray(0));
		GLCALL(glUseProgram(0));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	void MaterialLighting::OnUpdate(float deltaTime, GLFWwindow* window)
	{
		m_Camera->OnUpdate(deltaTime, window);


		m_YRotation += m_Increment;
	}

	void MaterialLighting::OnRender()
	{
		GLCALL(glClearColor(0.f, 0.f, 0.f, 1.f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_Texture1->Bind(0);
		m_Texture2->Bind(1);
		
		Renderer renderer;
		// Draw the cube
		{
			glm::mat4 model = glm::rotate(glm::mat4(1.f), glm::radians(m_YRotation), glm::vec3(0.f, 1.f, 0.f));
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_Model", model);
			m_Shader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
			m_Shader->SetUniformMat4f("u_Projection", m_Camera->GetProjectionMatrix());
			m_Shader->SetUniform1f("u_Material.shininess", m_MaterialShininess);

			m_Shader->SetUniform3f("u_Light.position", m_LightPosition[0], m_LightPosition[1], m_LightPosition[2]);
			m_Shader->SetUniform3f("u_Light.ambient", m_LightAmbient[0], m_LightAmbient[1], m_LightAmbient[2]);
			m_Shader->SetUniform3f("u_Light.diffuse", m_LightDiffuse[0], m_LightDiffuse[1], m_LightDiffuse[2]);
			m_Shader->SetUniform3f("u_Light.specular", m_LightSpecular[0], m_LightSpecular[1], m_LightSpecular[2]);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		// Draw the point light
		{
			glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(m_LightPosition[0], m_LightPosition[1], m_LightPosition[2]));
			model = glm::scale(model, glm::vec3(0.2f));
			glm::mat4 mvp = m_Camera->GetViewProjectionMatrix() * model;
			m_PointLightShader->Bind();
			m_PointLightShader->SetUniformMat4f("u_Model", model);
			m_PointLightShader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());
			m_PointLightShader->SetUniformMat4f("u_Projection", m_Camera->GetProjectionMatrix());
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_PointLightShader);
		}
	}

	void MaterialLighting::OnImGuiRender()
	{
		m_Camera->OnImGuiRender();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat("Rotation Rate ", &m_Increment, 0.f, 1.f);
		ImGui::DragFloat3("Light Position ", m_LightPosition, 0.1f, -100.f, 100.f);
		ImGui::DragFloat3("Light Ambient ", m_LightAmbient, 0.005f, 0.f, 1.f);
		ImGui::DragFloat3("Light Diffuse ", m_LightDiffuse, 0.005f, 0.f, 1.f);
		ImGui::DragFloat3("Light Specular ", m_LightSpecular, 0.005f, 0.f, 1.f);
		ImGui::Spacing();
		ImGui::DragFloat("Shininess", &m_MaterialShininess, 1.f, 1.f, 100.f);

	}
}