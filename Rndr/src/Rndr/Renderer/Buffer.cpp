#include "Buffer.h"

#include "Renderer.h"
#include "Rndr/Log.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Rndr
{
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		// return new OpenGLVertexBuffer(vertices, size);
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:      RNDR_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:    return std::make_shared<OpenGLVertexBuffer>(vertices, size);
			// case RendererAPI::API::Vulkan:    RNDR_CORE_ASSERT(false, "RendererAPI::API::Vulkan is currently not supported!"); return nullptr;
		}

		RNDR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:      RNDR_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:    return std::make_shared<OpenGLIndexBuffer>(indices, count);
			// case RendererAPI::API::Vulkan:    RNDR_CORE_ASSERT(false, "RendererAPI::API::Vulkan is currently not supported!"); return nullptr;
		}

		RNDR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}