#pragma once
#include "glm/glm.hpp"

#include "Scene.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "OrthographicCamera.h"

#include <memory>

namespace scene
{
	struct Vertex
	{
		float Position[3];
		float TexCoords[2];
		float TexID;
	};

	class BatchRenderer : public Scene
	{
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture1;
		std::unique_ptr<Texture> m_Texture2;
		std::unique_ptr<OrthographicCamera> m_Camera;

		float m_Quad0Position[2] = { -1.5f, 0.f };
		float m_Quad1Position[2] = { 0.f, 0.f };
		float m_Quad2Position[2] = { 1.5f, 0.f };

		int m_NumberOfQuads;

	public:
		BatchRenderer();
		~BatchRenderer();

		void OnUpdate(float deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		static std::array<Vertex, 4> CreateQuad(float x, float y, float texid);
		static std::array<int, 6> CreateIndices(int startingIndex);
	};
}