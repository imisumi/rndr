#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Rndr
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}