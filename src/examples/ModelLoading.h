#pragma once
#include "glm/glm.hpp"

#include "Scene.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "PerspectiveCamera.h"

#include <memory>
#include <vector>

namespace scene
{
	struct VertexData
	{
		glm::vec3 vertex;
		glm::vec3 normal;
		glm::vec2 uv;
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
		std::vector<VertexData> m_VertexData;
		std::unique_ptr<PerspectiveCamera> m_Camera;


		Light m_Light;
		Material m_Material;

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
	};
}