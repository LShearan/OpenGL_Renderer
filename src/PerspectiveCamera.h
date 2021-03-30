#pragma once

#include "glm/glm.hpp"

struct GLFWwindow;

namespace scene
{
	class PerspectiveCamera
	{
	private:
		float m_FOV, m_AspectRatio, m_zNear, m_zFar;
		float m_CameraSpeed, m_Rotation;
		glm::vec3 m_Position, m_ForwardVector, m_RightVector, m_UpVector;
		glm::mat4 m_Projection, m_View, m_ViewProjectionMatrix;
	public:
		PerspectiveCamera(const glm::vec3& pos, float fov, float aspectRatio, float zNear, float zFar);
		~PerspectiveCamera();

		void OnUpdate(float deltaTime, GLFWwindow* window);
		void OnImGuiRender();

		void SetProjection(float fov, float aspectRatio, float zNear, float zFar);
		
		inline glm::mat4 GetViewProjectionMatrix() { return m_ViewProjectionMatrix; }
		inline const glm::vec3& GetPosition() const { return m_Position; }
	private:
		void UpdateViewMatrix();
	};
}