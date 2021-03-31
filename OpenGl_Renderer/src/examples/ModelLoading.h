#pragma once
#include "glm/glm.hpp"

#include "Scene.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "PerspectiveCamera.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <memory>
#include <vector>

namespace scene
{
	struct VertexD
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
	};

	struct Material
	{
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		float Shininess;
	};

	struct Light
	{
		glm::vec3 Position;
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
	};


	class ModelLoader : public Scene
	{
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<PerspectiveCamera> m_Camera;

		std::vector<VertexD> m_VertexData;
		std::vector<TextureLayout> m_LoadedTextures;
		std::vector<Mesh> m_Meshes;
		Light m_Light;

		std::string m_Directory;


		float m_YRotation;
		float m_Increment;
	public:
		ModelLoader();
		~ModelLoader();

		void OnUpdate(float deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		bool LoadOBJModel(const std::string& filepath);
		void LoadModel(const std::string& filepath);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, int index);
		std::vector<TextureLayout> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}