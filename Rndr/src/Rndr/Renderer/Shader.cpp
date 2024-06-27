#include "Shader.h"

#include "Rndr/Log.h"

#include <vector>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include "Rndr/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Rndr
{
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    RNDR_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLShader(vertexSrc, fragmentSrc);
		}
		RNDR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}