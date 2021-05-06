#include "PerspectiveCamera.h"

#include "imgui/imgui.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

#include <algorithm>


namespace scene
{
	PerspectiveCamera::PerspectiveCamera(const glm::vec3& pos, float fov, float aspectRatio, float zNear, float zFar)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_zNear(zNear), m_zFar(zFar),
		m_ForwardVector(glm::vec3(0.f, 0.f, -1.f)), m_RightVector(glm::vec3(1.f, 0.f, 0.f)),
		m_UpVector(glm::vec3(0.f, 1.f, 0.f)),
		m_Position(pos),
		m_Rotation(0.f), m_CameraSpeed(0.04f),
		m_Pitch(0.f),m_Yaw(0.f),m_Roll(0.f)
	{
		m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_zNear, m_zFar);
		m_View = glm::lookAt(pos, m_Position + m_ForwardVector, m_UpVector);

		m_ViewProjectionMatrix = m_Projection * m_View;
	}

	PerspectiveCamera::~PerspectiveCamera()
	{
	}

	void PerspectiveCamera::OnUpdate(float deltaTime, GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_A))
		{
			m_Position.x -= m_CameraSpeed;
			UpdateViewMatrix();
		}
		if (glfwGetKey(window, GLFW_KEY_D))
		{
			m_Position.x += m_CameraSpeed;
			UpdateViewMatrix();
		}
		if (glfwGetKey(window, GLFW_KEY_W))
		{
			m_Position.z -= m_CameraSpeed;
			UpdateViewMatrix();
		}
		if (glfwGetKey(window, GLFW_KEY_S))
		{
			m_Position.z += m_CameraSpeed;
			UpdateViewMatrix();
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE))
		{
			m_Position.y += m_CameraSpeed;
			UpdateViewMatrix();
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
		{
			m_Position.y -= m_CameraSpeed;
			UpdateViewMatrix();
		}

	}

	void PerspectiveCamera::OnImGuiRender()
	{
		ImGui::Text("Camera Controls");
		ImGui::Text("WASD to Move Forward Back Left and Right");
		ImGui::Text("SPACE/LEFT SHIFT to Move Up and Down");
		ImGui::Text("Camera Position - %f,%f,%f", GetPosition().x, GetPosition().y, GetPosition().z);
	}

	void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float zNear, float zFar)
	{
		m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_zNear, m_zFar);
	}

	void PerspectiveCamera::UpdateViewMatrix()
	{
		m_View = glm::lookAt(m_Position, m_Position + m_ForwardVector, m_UpVector);
		m_ViewProjectionMatrix = m_Projection * m_View;
	}
}