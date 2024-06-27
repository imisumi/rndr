#include "VertexArray.h"

#include "Renderer.h"

#include "Rndr/Log.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Rndr
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    RNDR_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLVertexArray();
		}
		RNDR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}