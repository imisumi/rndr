#pragma once

#include <cstdint>
#include <string>
#include "Rndr/Core/Base.h"

#include <filesystem>

#include "Rndr/Assets/Asset.h"

#include <glad/glad.h>

namespace Rndr
{
	// enum class Texture2DSpecification
	// {
	// 	// RGBA8,
	// 	// RGBA16F,
	// 	// RGBA32F,
	// 	// RGB8,
	// 	// RGB16F,
	// 	// RGB32F,
	// 	// RG8,
	// 	// RG16F,
	// 	// RG32F,
	// 	// R8,
	// 	// R16F,
	// 	// R32F,
	// 	// Depth,
	// 	// DepthStencil
	// };

	enum class TextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RGBA16F,
		RGBA32F,
		RGB8,
		RGB16F,
		RGB32F,
		RG8,
		RG16F,
		RG32F,
		R8,
		R16F,
		R32F,
		Depth,
		DepthStencil,

		// Defaults
		Depth24Stencil8 = DepthStencil
	};

	struct Texture2DSpecification
	{
		uint32_t Width = 0, Height = 0;
		TextureFormat Format = TextureFormat::None;

		uint32_t MinFilter = GL_LINEAR;
		uint32_t MagFilter = GL_LINEAR;
		uint32_t WrapS = GL_REPEAT;
		uint32_t WrapT = GL_REPEAT;
	};

	class Texture : public Asset
	{
	public:
		virtual ~Texture() = default;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetTextureID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D() = default;
		Texture2D(uint32_t width, uint32_t height);
		Texture2D(const std::string& path);
		Texture2D(const std::filesystem::path& path);
		~Texture2D();

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		uint32_t GetTextureID() const
		{
			// RNDR_CORE_INFO("Getting texture ID for {0}", m_Path);
			return m_RendererID;
		}

		void SetData(void* data, uint32_t size);

		void Bind(uint32_t slot = 0) const;

		bool operator==(const Texture& other) const
		{
			return m_RendererID == ((Texture2D&)other).m_RendererID;
		}

		static Ref<Texture2D> Create();
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(const std::filesystem::path& path);

		AssetType GetType() const { return AssetType::Texture; }

	private:
		Texture2DSpecification m_Specification;

		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID = 0;
		uint32_t m_InternalFormat, m_DataFormat, m_nChannels;
	};
}