#include "VertexArray.h"

#include "Renderer.h"

#include "Rndr/Core/Log.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Rndr
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    RNDR_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
		}
		RNDR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}