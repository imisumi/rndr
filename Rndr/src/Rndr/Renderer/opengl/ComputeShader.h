#pragma once

#include "Rndr/Core/Base.h"

#include <string>
#include <glm/glm.hpp>

#include <unordered_map>

#include <glad/glad.h>

#include <array>
#include <tuple>

namespace Rndr
{
	class ComputeShader
	{
	public:
		ComputeShader();
		ComputeShader(const std::string& filename);
		~ComputeShader();

		void Bind() const;
		void Unbind() const;

		void Dispatch(uint32_t x, uint32_t y, uint32_t z);

		static Ref<ComputeShader> Create(const std::string& filepath);






		int32_t GetUniformLocation(const std::string& name);

		void SetBool(const std::string& name, bool value);

		void SetUnsignedInt(const std::string& name, uint32_t value);

		void SetInt(const std::string& name, int value);
		void SetIntArray(const std::string& name, int* values, uint32_t count);
		
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, const glm::vec2& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);

		void SetMat3(const std::string& name, const glm::mat3& value);
		void SetMat4(const std::string& name, const glm::mat4& value);





		//? Storage Buffer
		std::tuple<uint32_t, bool> GetBufferID(uint32_t slot);
		void SetStorageBuffer(uint32_t slot, GLsizeiptr size, const void* data, GLenum usage);
	public:
		uint32_t m_RendererID = 0;
		std::unordered_map<std::string, int32_t> m_UniformLocationCache;

		static constexpr size_t m_MaxStorageBuffers = 16;
		std::array<uint32_t, m_MaxStorageBuffers> m_StorageBufferSlots;
	};
}