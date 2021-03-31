#include "Scene.h"
#include "imgui/imgui.h"

namespace scene
{
	SceneMenu::SceneMenu(Scene*& currentTestPtr)
		:m_CurrentScene(currentTestPtr)
	{
	}
	void SceneMenu::OnImGuiRender()
	{
		for (auto& scene : m_Scenes)
		{
			if (ImGui::Button(scene.first.c_str()))
			{
				m_CurrentScene = scene.second();
			}
		}
	}
}