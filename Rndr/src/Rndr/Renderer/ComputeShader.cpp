#include "ComputeShader.h"

#include "Rndr/Core/Log.h"


#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <filesystem>

namespace Rndr
{
	static std::string ReadFile(const std::string& filepath)
	{
		std::ifstream file(filepath);
		if (!file.is_open())
			RNDR_CORE_ASSERT(false, "Could not open file '{0}'", filepath);
		

		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	static std::string processIncludes(const std::string& input, const std::filesystem::path& basePath)
	{
		std::stringstream output;
		std::istringstream stream(input);
		std::string line;

		while (std::getline(stream, line)) {
			if (line.find("#include \"") != std::string::npos) {
				size_t start = line.find("#include \"") + 10;
				size_t end = line.find("\"", start);
				if (end == std::string::npos)
					RNDR_CORE_ASSERT(false, "Malformed #include directive: {0}", line);

				std::string includePath = line.substr(start, end - start);
				std::filesystem::path fullPath = basePath / includePath;

				if (!std::filesystem::exists(fullPath))
					RNDR_CORE_ASSERT(false, "Include file does not exist: {0}", fullPath.string());

				std::string includedContent = ReadFile(fullPath.string());
				includedContent = processIncludes(includedContent, fullPath.parent_path()); // Recursively process includes
				output << includedContent;
			} else {
				output << line << '\n';
			}
		}

		return output.str();
	}

	static std::string loadAndProcessFile(const std::string& filepath)
	{
		std::filesystem::path path(filepath);
		std::string content = ReadFile(filepath);
		return processIncludes(content, path.parent_path());
	}

	static uint32_t compileComputeShader(const std::string& source)
	{
		uint32_t id = glCreateShader(GL_COMPUTE_SHADER);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
		
			glGetShaderInfoLog(id, length, &length, message);
			// extract the number between the () in the message
			std::string msg(message);
			std::string number = msg.substr(msg.find("(") + 1, msg.find(")") - msg.find("(") - 1);
			int targetLine = std::stoi(number);
			std::string::size_type start = 0, end;
			int lineNumber = 0;
			while (lineNumber < targetLine) {
				end = source.find('\n', start);
				if (end == std::string::npos) {
					// If no more newlines and not reached target, break
					if (++lineNumber < targetLine) {
						// std::cout << "The string contains fewer than 10 lines." << std::endl;
						// RNDR_CORE_ERROR("The string contains fewer than 10 lines.");
					}
					break;
				}
				++lineNumber;
				if (lineNumber == targetLine) {
					// std::cout << "The 10th line is: " << source.substr(start, end - start) << std::endl;
					RNDR_CORE_ERROR("{0}", source.substr(start, end - start));
					break;
				}
				start = end + 1; // Move start to the next line
			}
			// print line number from source

			// RNDR_CORE_ERROR("Failed to compile compute shader! Line: {0}", number);
			RNDR_CORE_ERROR("Failed to compile compute shader!");
			RNDR_CORE_ERROR("{0}", message);
			glDeleteShader(id);

			RNDR_CORE_ASSERT(false, "Failed to compile compute shader!");
			return 0;
		}

		return id;
	}

	ComputeShader::ComputeShader()
	{
	}
	
	ComputeShader::~ComputeShader()
	{
		glDeleteProgram(m_RendererID);
	}

	ComputeShader::ComputeShader(const std::string& filename)
	{
		// std::string source = ReadFile(filename);
		std::string source = loadAndProcessFile(filename);

		uint32_t computeShader = compileComputeShader(source);

		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, computeShader);
		glLinkProgram(m_RendererID);

		int result;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetProgramInfoLog(m_RendererID, length, &length, message);
			RNDR_CORE_ERROR("Failed to link compute shader program!");
			RNDR_CORE_ERROR("{0}", message);
			glDeleteProgram(m_RendererID);

			RNDR_CORE_ASSERT(false, "Failed to link compute shader program!");
			return;
		}
	}

	void ComputeShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void ComputeShader::Unbind() const
	{
		glUseProgram(0);
	}

	void ComputeShader::Dispatch(uint32_t x, uint32_t y, uint32_t z)
	{
		glDispatchCompute(x, y, z);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}


	
	Ref<ComputeShader> ComputeShader::Create(const std::string& filepath)
	{
		return CreateRef<ComputeShader>(filepath);
	}



















	uint32_t ComputeShader::GetUniformLocation(const std::string& name)
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

	void ComputeShader::SetBool(const std::string& name, bool value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform1i(location, value);
	}

	void ComputeShader::SetUnsignedInt(const std::string& name, uint32_t value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform1ui(location, value);
	}

	void ComputeShader::SetInt(const std::string& name, int value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform1i(location, value);
	}

	void ComputeShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform1iv(location, count, values);
	}

	void ComputeShader::SetFloat(const std::string& name, float value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform1f(location, value);
	}

	void ComputeShader::SetFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform2f(location, values.x, values.y);
	}

	void ComputeShader::SetFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform3f(location, values.x, values.y, values.z);
	}

	void ComputeShader::SetFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void ComputeShader::SetMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void ComputeShader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1)
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}