#include "OrthographicCamera.h"

#include "imgui/imgui.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

#include <algorithm>

namespace scene
{

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_Projection(glm::ortho(left, right, bottom, top, -1.f, 1.f))
		, m_View(1.f),
		m_Position(0.f, 0.f, 0.f),
		m_Rotation(0.f), m_CameraSpeed(0.04f), m_ZoomLevel(1.f), m_AspectRatio(16.f / 9.f)
	{
		m_ViewProjectionMatrix = m_Projection * m_View;
	}

	OrthographicCamera::~OrthographicCamera()
	{
	}

	void OrthographicCamera::OnUpdate(float deltaTime, GLFWwindow* window)
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
			m_Position.y += m_CameraSpeed;
			UpdateViewMatrix();
		}
		if (glfwGetKey(window, GLFW_KEY_S))
		{
			m_Position.y -= m_CameraSpeed;
			UpdateViewMatrix();
		}
		if (glfwGetKey(window, GLFW_KEY_E))
		{
			m_ZoomLevel -= 0.1f;
			m_ZoomLevel = std::max(m_ZoomLevel, 0.1f);
			SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
			UpdateViewMatrix();
		}
		if (glfwGetKey(window, GLFW_KEY_Q))
		{
			m_ZoomLevel += 0.1f;
			m_ZoomLevel = std::max(m_ZoomLevel, 0.1f);
			SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
			UpdateViewMatrix();
		}

	}

	void OrthographicCamera::OnImGuiRender()
	{
		ImGui::Text("Camera Controls");
		ImGui::Text("WASD to Move");
		ImGui::Text("QE to Zoom");
		ImGui::Text("Camera Position - %f,%f,%f", GetPosition().x, GetPosition().y, GetPosition().z);

	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_Projection = glm::ortho(left, right, bottom, top, -1.f, 1.f);
	}

	void OrthographicCamera::UpdateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), m_Position) *
			glm::rotate(glm::mat4(1.f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_View = glm::inverse(transform);
		m_ViewProjectionMatrix = m_Projection * m_View;
	}
}