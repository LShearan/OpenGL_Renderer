#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>

#include "glm/glm.hpp"

struct GLFWwindow;


namespace scene
{
	class Scene
	{
	public:
		Scene() {}
		virtual ~Scene() {}

		virtual void OnUpdate(float deltaTime, GLFWwindow* window) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class SceneMenu : public Scene
	{
	private:
		std::vector<std::pair<std::string, std::function<Scene*()>>> m_Scenes;
		Scene*& m_CurrentScene;
	public:
		SceneMenu(Scene*& currentTestPtr);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registered new Scene " << name << std::endl;
			m_Scenes.push_back(std::make_pair(name, []() { return new T; }));
		}
	};
}