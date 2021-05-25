#pragma once
#include "Scene.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Mesh.h"
#include "PerspectiveCamera.h"
#include "Shader.h"

namespace scene
{

	class SimpleLigthing : public Scene
	{
	private:
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Shader> m_PointLightShader;
		std::unique_ptr<PerspectiveCamera> m_Camera;
		std::unique_ptr<Mesh> m_Mesh;

		float m_YRotation;
		float m_Increment;
		float m_LightPosition[3];
	public:
		SimpleLigthing();
		~SimpleLigthing();

		void OnUpdate(float deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}