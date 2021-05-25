#pragma once

#include "glm/glm.hpp"

#include "Scene.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Mesh.h"
#include "PerspectiveCamera.h"

#include <memory>

namespace scene
{
	class TexturedCube : public Scene
	{
	private:
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<PerspectiveCamera> m_Camera;
		std::unique_ptr<Mesh> m_Mesh;

		float m_YRotation;
		float m_Increment;
	public:
		TexturedCube();
		~TexturedCube();

		void OnUpdate(float deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}