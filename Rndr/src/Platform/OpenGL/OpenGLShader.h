#pragma once

#include "Rndr/Renderer/Shader.h"

#include <glm/glm.hpp>

#include <glad/glad.h>

namespace Rndr
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override { return m_Name; }

		// Set uniforms
		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string ReadFile(const std::string& filepath);
		// void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
		void CreateProgram(GLuint vertexShader, GLuint fragmentShader);
		GLuint CompileShader(GLenum type, const std::string& source);
		std::pair<std::string, std::string> extractVersionDirective(const std::string& shaderCode);
	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}