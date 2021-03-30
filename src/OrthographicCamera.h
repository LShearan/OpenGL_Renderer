#pragma once
#include "glm/glm.hpp"

struct GLFWwindow;

namespace scene
{
	class OrthographicCamera
	{
	private:
		float m_Rotation, m_CameraSpeed, m_ZoomLevel, m_AspectRatio;
		glm::vec3 m_Position;
		glm::mat4 m_Projection, m_View, m_ViewProjectionMatrix;
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		~OrthographicCamera();

		void OnUpdate(float deltaTime, GLFWwindow* window);
		void OnImGuiRender();

		void SetProjection(float left, float right, float bottom, float top);

		inline const glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		inline const glm::vec3& GetPosition() const { return m_Position; }
	private:
		void UpdateViewMatrix();

	};
}