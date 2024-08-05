
#pragma once
#include "Rndr/Renderer/Shader.h"

#include <vector>

#include <glad/glad.h>

namespace Rndr
{
	class ViewportGrid
	{
	public:
		ViewportGrid()
		{
			m_Spacing = 0.1f; // Example spacing
			m_NumLines = 21;    // Example: odd number for the center line

			// Generate lines along X-axis
			for (int i = -m_NumLines/2; i <= m_NumLines/2; ++i) {
				m_Vertices.push_back(-1.0f); m_Vertices.push_back(i * m_Spacing); m_Vertices.push_back(0.0f);
				m_Vertices.push_back( 1.0f); m_Vertices.push_back(i * m_Spacing); m_Vertices.push_back(0.0f);
			}

			// Generate lines along Y-axis
			for (int i = -m_NumLines/2; i <= m_NumLines/2; ++i) {
				m_Vertices.push_back(i * m_Spacing); m_Vertices.push_back(-1.0f); m_Vertices.push_back(0.0f);
				m_Vertices.push_back(i * m_Spacing); m_Vertices.push_back( 1.0f); m_Vertices.push_back(0.0f);
			}

			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);

			glBindVertexArray(m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(GLfloat), m_Vertices.data(), GL_STATIC_DRAW);

			// Position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			m_GridShader = Shader::Create("Editor/assets/shaders/GridShader.glsl");
			// bind?
		}


		~ViewportGrid()
		{
			glDeleteVertexArrays(1, &m_VAO);
			glDeleteBuffers(1, &m_VBO);
		}


		void Draw()
		{
			m_GridShader->Bind();
			// glBindVertexArray(m_VAO);
			// glDrawArrays(GL_LINES, 0, m_Vertices.size() / 3);
			// glBindVertexArray(0);
		}

	private:
		Ref<Shader> m_GridShader;
		GLuint m_VBO, m_VAO;
		uint32_t m_RendererID;
		BufferLayout m_Layout;

		float m_Spacing;
		int m_NumLines;
		std::vector<GLfloat> m_Vertices;
	};
}