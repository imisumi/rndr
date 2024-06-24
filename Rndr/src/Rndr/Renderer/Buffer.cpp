#include "Buffer.h"

#include "Renderer.h"
#include "Rndr/Log.h"

#include "Renderer.h"
#include "Rndr/Platform/OpenGL/OpenGLBuffer.h"

namespace Rndr
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		// return new OpenGLVertexBuffer(vertices, size);
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:      RNDR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:    return new OpenGLVertexBuffer(vertices, size);
			case RendererAPI::Vulkan:    RNDR_CORE_ASSERT(false, "RendererAPI::Vulkan is currently not supported!"); return nullptr;
		}

		RNDR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:      RNDR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:    return new OpenGLIndexBuffer(indices, count);
			case RendererAPI::Vulkan:    RNDR_CORE_ASSERT(false, "RendererAPI::Vulkan is currently not supported!"); return nullptr;
		}

		RNDR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}