#include "ModelLoading.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

namespace scene {

	ModelLoader::ModelLoader()
		: m_YRotation(0.f), m_Increment(0.05f)
	{
		if(LoadOBJModel("res/models/Mandalorian.obj"))
		{
			/* Enable Blending */
			GLCALL(glEnable(GL_BLEND));
			GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

			/* Enable Depth Test */
			GLCALL(glEnable(GL_DEPTH_TEST));
			GLCALL(glDepthFunc(GL_LESS));

			/* Create light and Material*/
			m_Light.Position = glm::vec3(0.f, 10.f, 100.f);
			m_Light.Ambient = glm::vec3(0.2f, 0.2f, 0.2f);
			m_Light.Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
			m_Light.Specular = glm::vec3(1.f, 1.f, 1.f);

			m_Material.Ambient = glm::vec3(1.f, 0.5f, 0.31f);
			m_Material.Diffuse = glm::vec3(1.f, 0.5f, 0.31f);
			m_Material.Specular = glm::vec3(.5f, 0.5f, 0.5f);
			m_Material.Shininess = 32.f;

			m_Camera = std::make_unique<PerspectiveCamera>(glm::vec3(2.5f, 2.f, 50.f), 70.f, 16.f / 9.f, 0.1f, 1000.f);

			m_VAO = std::make_unique<VertexArray>();
			m_VertexBuffer = std::make_unique<VertexBuffer>(&m_VertexData[0], m_VertexData.size() * sizeof(VertexData));
			VertexBufferLayout layout;
			layout.Push<float>(3);
			layout.Push<float>(3);
			layout.Push<float>(2);

			m_VAO->AddBuffer(*m_VertexBuffer, layout);
			
			m_Shader = std::make_unique<Shader>("res/shaders/ModelLoader.shader");
			m_Shader->Bind();
			m_Shader->SetUniform3f("u_Material.Ambient", m_Material.Ambient.x, m_Material.Ambient.y, m_Material.Ambient.z);
			m_Shader->SetUniform3f("u_Material.Diffuse", m_Material.Diffuse.x, m_Material.Diffuse.y, m_Material.Diffuse.z);
			m_Shader->SetUniform3f("u_Material.Specular", m_Material.Specular.x, m_Material.Specular.y, m_Material.Specular.z);
			m_Shader->SetUniform1f("u_Material.Shininess", m_Material.Shininess);
			m_Shader->SetUniform3f("u_Light.Position", m_Light.Position.x, m_Light.Position.y, m_Light.Position.z);
			m_Shader->SetUniform3f("u_Light.Ambient", m_Light.Ambient.x, m_Light.Ambient.y, m_Light.Ambient.z);
			m_Shader->SetUniform3f("u_Light.Diffuse", m_Light.Diffuse.x, m_Light.Diffuse.y, m_Light.Diffuse.z);
			m_Shader->SetUniform3f("u_Light.Specular", m_Light.Specular.x, m_Light.Specular.y, m_Light.Specular.z);
		}
	}

	ModelLoader::~ModelLoader()
	{
		GLCALL(glDisable(GL_BLEND));
		GLCALL(glDisable(GL_DEPTH_TEST));
		GLCALL(glBindVertexArray(0));
		GLCALL(glUseProgram(0));
	}

	void ModelLoader::OnUpdate(float deltaTime, GLFWwindow* window)
	{
		m_Camera->OnUpdate(deltaTime, window);

		m_YRotation += m_Increment;
	}

	void ModelLoader::OnRender()
	{
		GLCALL(glClearColor(0.f, 0.f, 0.f, 1.f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


		Renderer renderer;
		{
			glm::vec3 camPos = m_Camera->GetPosition();
			glm::mat4 model = glm::rotate(glm::mat4(1.f), glm::radians(m_YRotation), glm::vec3(0.f, 1.f, 0.f));
			glm::mat4 mvp = m_Camera->GetViewProjectionMatrix() * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			m_Shader->SetUniformMat4f("u_ModelMatrix", model);
			m_Shader->SetUniform3f("u_ViewPos", camPos.x, camPos.y, camPos.z);
			m_Shader->SetUniform3f("u_Material.Ambient", m_Material.Ambient.x, m_Material.Ambient.y, m_Material.Ambient.z);
			m_Shader->SetUniform3f("u_Material.Diffuse", m_Material.Diffuse.x, m_Material.Diffuse.y, m_Material.Diffuse.z);
			m_Shader->SetUniform3f("u_Material.Specular", m_Material.Specular.x, m_Material.Specular.y, m_Material.Specular.z);
			m_Shader->SetUniform1f("u_Material.Shininess", m_Material.Shininess);
			m_Shader->SetUniform3f("u_Light.Position", m_Light.Position.x, m_Light.Position.y, m_Light.Position.z);
			m_Shader->SetUniform3f("u_Light.Ambient", m_Light.Ambient.x, m_Light.Ambient.y, m_Light.Ambient.z);
			m_Shader->SetUniform3f("u_Light.Diffuse", m_Light.Diffuse.x, m_Light.Diffuse.y, m_Light.Diffuse.z);
			m_Shader->SetUniform3f("u_Light.Specular", m_Light.Specular.x, m_Light.Specular.y, m_Light.Specular.z);
			renderer.DrawArray(*m_VAO, *m_Shader, 0, m_VertexData.size());
		}
	}

	void ModelLoader::OnImGuiRender()
	{
		m_Camera->OnImGuiRender();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat("Rotation Rate: ", &m_Increment, 0.f, 1.f);
		ImGui::Text("Light");
		ImGui::SliderFloat3("Positon ", &m_Light.Position.x, -100.f, 100.f);
		ImGui::SliderFloat3("Ambient ", &m_Light.Ambient.x, 0.f, 1.f);
		ImGui::SliderFloat3("Diffuse ", &m_Light.Diffuse.x, 0.f, 1.f);
		ImGui::SliderFloat3("Specular ", &m_Light.Specular.x, -0.f, 1.f);
		ImGui::Text("Material");
		ImGui::SliderFloat3("Ambient ", &m_Material.Ambient.x, 0.f, 1.f);
		ImGui::SliderFloat3("Diffuse ", &m_Material.Diffuse.x, 0.f, 1.f);
		ImGui::SliderFloat3("Specular ", &m_Material.Specular.x, 0.f, 1.f);
		ImGui::SliderFloat("Shininess ", &m_Material.Shininess, 0.f, 50.f);
	}


	bool ModelLoader::LoadOBJModel(const std::string& filepath)
	{
		std::vector<VertexData> data;

		FILE* file = fopen(filepath.c_str(), "r");
		if (file == NULL)
		{
			std::cout << "Failed to open File - " << filepath << std::endl;
			return false;
		}

		std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		std::vector< glm::vec3 > temp_vertices;
		std::vector< glm::vec2 > temp_uvs;
		std::vector< glm::vec3 > temp_normals;

		while (1)
		{
			char lineHeader[128];

			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF) break;

			if (strcmp(lineHeader, "v") == 0)
			{
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0)
			{
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				temp_uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0)
			{
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0)
			{
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9)
				{
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return false;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}

		}

		for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			unsigned int vertexIndex = vertexIndices[i];
			glm::vec3 vertex = temp_vertices[vertexIndex - 1];
			glm::vec2 uv = temp_uvs[vertexIndex - 1];
			glm::vec3 normal = temp_normals[vertexIndex - 1];

			m_VertexData.push_back({ vertex,normal,uv });
		}

		return true;
	}
}
