#include "OpenGLShader.h"

#include "Rndr/Core/Log.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

namespace Rndr
{
	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			RNDR_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::pair<std::string, std::string> OpenGLShader::extractVersionDirective(const std::string& shaderCode)
	{
		std::istringstream stream(shaderCode);
		std::string line;
		std::string versionDirective;
		std::string remainingCode;
		bool versionFound = false;

		while (std::getline(stream, line)) {
			if (!versionFound && line.find("#version") != std::string::npos) {
				versionDirective = line + "\n";
				versionFound = true;
			} else {
				remainingCode += line + "\n";
			}
		}

		return {versionDirective, remainingCode};
	}

	void OpenGLShader::CreateProgram(GLuint vertexShader, GLuint fragmentShader)
	{
		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);
		glLinkProgram(m_RendererID);

		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			RNDR_CORE_ERROR("{0}", infoLog.data());
			RNDR_CORE_ASSERT(false, "OpenGLShader link failure!");
			return;
		}

		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
	}

	GLuint OpenGLShader::CompileShader(GLenum shaderType, const std::string& shaderCode)
	{
		std::string shaderTypeDefine;
		std::string shaderSource;

		if (shaderType == GL_VERTEX_SHADER)
			shaderTypeDefine = "#define _TYPE_VERT_SHADER\n";
		else if (shaderType == GL_FRAGMENT_SHADER)
			shaderTypeDefine = "#define _TYPE_FRAG_SHADER\n";
		else
		{
			RNDR_CORE_ASSERT(false, "Unknown shader type");
			return 0;
		}

		auto [versionDirective, remainingCode] = extractVersionDirective(shaderCode);
		std::string finalShaderCode = versionDirective + shaderTypeDefine + remainingCode;
		const char* codeArray = finalShaderCode.c_str();
		// const char* codeArray = shaderSource.c_str();

		// std::cout << codeArray << std::endl;
		// std::string finalCode = codeArray;
		// RNDR_CORE_INFO("\n{0}", finalCode);
		// exit(0);

		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &codeArray, NULL);
		glCompileShader(shader);

		// check for shader compile errors
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			std::vector<GLchar> log(length);
			glGetShaderInfoLog(shader, length, &length, &log[0]);
			std::string type = shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment";
			RNDR_CORE_ERROR("Failed to compile {0} shader: {1}", type ,log.data());
			RNDR_CORE_ASSERT(false, "Shader compilation failure!");
			glDeleteShader(shader);
			return 0;
		}

		return shader;
	}

	/*
		This constructor takes a file containing both vertex and fragment shaders.
		To use this define your OpenGL version once at the top and wrap the vertex and fragment
		shaders in #ifdef _TYPE_VERT_SHADER and #ifdef _TYPE_FRAG_SHADER respectively.
	*/
	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);

		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, source);
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, source);

		CreateProgram(vertexShader, fragmentShader);

		std::filesystem::path path = filepath;
		m_Name = path.stem().string(); // Returns the file's name stripped of the extension.
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrc);
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

		CreateProgram(vertexShader, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}




	uint32_t OpenGLShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			RNDR_CORE_WARN("Warning: uniform '{0}' doesn't exist!", name);
			// std::cerr << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

		m_UniformLocationCache[name] = location;
		return location;
	}

	void OpenGLShader::SetBool(const std::string& name, bool value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform1i(location, value);
	}

	void OpenGLShader::SetUnsignedInt(const std::string& name, uint32_t value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform1ui(location, value);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform1i(location, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform1iv(location, count, values);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform1f(location, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}


} // namespace Rndr
