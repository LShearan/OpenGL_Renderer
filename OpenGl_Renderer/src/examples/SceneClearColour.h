#pragma once

#include "Scene.h"

namespace scene
{
	class ClearColour : public Scene
	{
	private:
		float m_ClearColour[4];

	public:
		ClearColour();
		~ClearColour();

		void OnUpdate(float deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}