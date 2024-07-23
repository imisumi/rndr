#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

#include <iostream>

namespace Rndr
{
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		// glDepthFunc(GL_LESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		// glEnable(GL_CULL_FACE);
		// glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount, uint32_t baseIndex, uint32_t baseVertex)
	{
		// uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();

		// uint32_t count = indexCount ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;

		// uint32_t count;
		// if (indexCount)
		// 	count = indexCount;
		// else
		// 	count = vertexArray->GetIndexBuffer()->GetCount();
		// std::cout << "OpenGLRendererAPI::DrawIndexed: count = " << count << std::endl;
		vertexArray->Bind();

		// GLsizeiptr offset = indexOffset * sizeof(uint32_t);
		// glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (const void*)offset);
		// glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);


		glDrawElementsBaseVertex(GL_TRIANGLES, 
			count, 
			GL_UNSIGNED_INT, 
			(void*)(sizeof(uint32_t) * baseIndex), 
			baseVertex);

		//TODO: TEMP use glDrawElementsBaseVertex
		// glBindTexture(GL_TEXTURE_2D, 0);
	}


	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount, float lineWidth)
	{
		vertexArray->Bind();
		glLineWidth(lineWidth);
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	// void OpenGLRendererAPI::SetLineWidth(float width)
	// {
	// 	glLineWidth(width);
	// }
}