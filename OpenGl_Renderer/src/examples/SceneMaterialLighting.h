#pragma once
#include "Scene.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "PerspectiveCamera.h"
#include "Shader.h"
#include "Texture.h"

namespace scene
{

	class MaterialLighting : public Scene
	{
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Shader> m_PointLightShader;
		std::unique_ptr<PerspectiveCamera> m_Camera;
		std::unique_ptr<Texture> m_Texture1, m_Texture2;

		// Light Properties
		float m_LightPosition[3];
		float m_LightAmbient[3];
		float m_LightDiffuse[3];
		float m_LightSpecular[3];

		// Material Properties
		float m_MaterialShininess;

		float m_YRotation;
		float m_Increment;
	public:
		MaterialLighting();
		~MaterialLighting();

		void OnUpdate(float deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}