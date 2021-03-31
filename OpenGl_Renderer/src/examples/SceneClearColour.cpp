#include "SceneClearColour.h"
#include "Renderer.h"
#include "imgui/imgui.h"

namespace scene
{

	ClearColour::ClearColour()
		:m_ClearColour{0.2f,0.3,0.9f,1.f}
	{
	}

	ClearColour::~ClearColour()
	{
	}

	void ClearColour::OnUpdate(float deltaTime, GLFWwindow* window)
	{
	}

	void ClearColour::OnRender()
	{
		GLCALL(glClearColor(m_ClearColour[0], m_ClearColour[1], m_ClearColour[2], m_ClearColour[3]));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));
	}

	void ClearColour::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Colour", m_ClearColour);
	}
}