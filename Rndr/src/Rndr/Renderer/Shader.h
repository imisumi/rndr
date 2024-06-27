#pragma once

#include <string>

namespace Rndr
{
	class Shader
	{
	public:
		~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);

		// virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& values) = 0;
	};
}