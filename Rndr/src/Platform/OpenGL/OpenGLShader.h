#pragma once

#include "Rndr/Renderer/opengl/Shader.h"

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

		virtual void SetName(const std::string& name) override { m_Name = name; }
		virtual const std::string& GetName() const override { return m_Name; }


		uint32_t GetUniformLocation(const std::string& name);

		virtual void SetBool(const std::string& name, bool value) override;

		virtual void SetUnsignedInt(const std::string& name, uint32_t value) override;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;

		virtual void SetMat3(const std::string& name, const glm::mat3& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		

	private:
		std::string ReadFile(const std::string& filepath);
		// void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
		void CreateProgram(GLuint vertexShader, GLuint fragmentShader);
		GLuint CompileShader(GLenum type, const std::string& source);
		std::pair<std::string, std::string> extractVersionDirective(const std::string& shaderCode);
	private:
		uint32_t m_RendererID;
		std::string m_Name;


		std::unordered_map<std::string, GLint> m_UniformLocationCache;
	};
}