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


		LoadModel("res/models/Backpack/backpack.obj");

		/* Enable Blending */
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/* Enable Depth Test */
		GLCALL(glEnable(GL_DEPTH_TEST));
		GLCALL(glDepthFunc(GL_LESS));

		/* Create a Point Light */
		m_Light.Position = glm::vec3(0.f, 10.f, 100.f);
		m_Light.Ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		m_Light.Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		m_Light.Specular = glm::vec3(1.f, 1.f, 1.f);

		/* Create a perspecti*/
		m_Camera = std::make_unique<PerspectiveCamera>(glm::vec3(2.5f, 2.f, 25.f), 70.f, 16.f / 9.f, 0.1f, 1000.f);


		m_Shader = std::make_unique<Shader>("res/shaders/Model3D.shader");
		m_Shader->Bind();

		m_DiffTexture = std::make_unique<Texture>(m_Directory + "/diffuse.jpg");
		m_SpecTexture = std::make_unique<Texture>(m_Directory + "/specular.jpg");
		m_Shader->SetUniform1i("u_Diffuse", 0);
		m_Shader->SetUniform1i("u_Specular", 1);
		m_Shader->SetUniform3f("u_Light.Position", m_Light.Position.x, m_Light.Position.y, m_Light.Position.z);
		m_Shader->SetUniform3f("u_Light.Ambient", m_Light.Ambient.x, m_Light.Ambient.y, m_Light.Ambient.z);
		m_Shader->SetUniform3f("u_Light.Diffuse", m_Light.Diffuse.x, m_Light.Diffuse.y, m_Light.Diffuse.z);
		m_Shader->SetUniform3f("u_Light.Specular", m_Light.Specular.x, m_Light.Specular.y, m_Light.Specular.z);

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

		
		m_DiffTexture->Bind(0);
		m_SpecTexture->Bind(1);

		{
			glm::vec3 camPos = m_Camera->GetPosition();
			glm::mat4 model = glm::rotate(glm::mat4(1.f), glm::radians(m_YRotation), glm::vec3(0.f, 1.f, 0.f));
			glm::mat4 mvp = m_Camera->GetViewProjectionMatrix() * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			m_Shader->SetUniformMat4f("u_ModelMatrix", model);
			m_Shader->SetUniform3f("u_Light.Position", m_Light.Position.x, m_Light.Position.y, m_Light.Position.z);
			m_Shader->SetUniform3f("u_Light.Ambient", m_Light.Ambient.x, m_Light.Ambient.y, m_Light.Ambient.z);
			m_Shader->SetUniform3f("u_Light.Diffuse", m_Light.Diffuse.x, m_Light.Diffuse.y, m_Light.Diffuse.z);
			m_Shader->SetUniform3f("u_Light.Specular", m_Light.Specular.x, m_Light.Specular.y, m_Light.Specular.z);
			for (int i = 0; i < m_Meshes.size(); i++)
				m_Meshes[i]->OnRender(*m_Shader);

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
	}


	bool ModelLoader::LoadOBJModel(const std::string& filepath)
	{
		std::vector<VertexLayout> data;

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

			//m_VertexData.push_back({ vertex,normal,uv });
		}

		return true;
	}

	void ModelLoader::LoadModel(const std::string& filepath)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR with ASSIMP " << import.GetErrorString() << std::endl;
			return;
		}
		m_Directory = filepath.substr(0, filepath.find_last_of('/'));

		
		ProcessNode(scene->mRootNode, scene);
	}
	void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene, 0));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	std::unique_ptr<Mesh> ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, int index)
	{
		std::vector<VertexLayout> vertices;
		vertices.reserve(mesh->mNumVertices);
		std::vector<unsigned int> indices;
		indices.reserve(mesh->mNumFaces);
		std::vector<TextureLayout> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			VertexLayout vertex;
			glm::vec3 pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertex.Position = pos;
			glm::vec3 norm(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			vertex.Normal = pos;
			glm::vec2 texc;

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 tex(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				texc.x = mesh->mTextureCoords[0][i].x;
				texc.x = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = tex;
			}
			else
			{
				texc.x = 0.f;
				texc.x = 0.f;
				vertex.TexCoords = glm::vec2(0.f, 0.f);
			}

			vertices.push_back(vertex);
			m_VertexData.push_back({pos,norm,texc});
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
				//m_IndicesData.push_back(face.mIndices[j]);
			}
		}

		/*if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<TextureLayout> diffuseMaps = LoadMaterialTextures(mat,
				aiTextureType_DIFFUSE, "u_Diffuse");

			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			

			std::vector<TextureLayout> specularMaps = LoadMaterialTextures(mat,
				aiTextureType_SPECULAR, "u_Specular");


			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			
		}*/

		return std::make_unique<Mesh>(vertices,indices);
	}
	void ModelLoader::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;
			for (unsigned int j = 0; j < m_LoadedTextures.size(); j++)
			{
				if (std::strcmp(m_LoadedTextures[j].Path.data(), str.C_Str()) == 0)
				{
					m_TextureLayouts.push_back(m_LoadedTextures[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				TextureLayout tex;
				tex.ID = i;
				tex.Type = typeName;
				tex.Path = m_Directory + "/" + str.C_Str();
				m_TextureLayouts.push_back(tex);
				m_LoadedTextures.push_back(tex);
			}
		}
	}
}
