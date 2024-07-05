#include "OpenGLTexture.h"

#include "Rndr/Core/Log.h"

#include <glad/glad.h>

#include <OpenImageIO/imageio.h>

#include <iostream>
#include <filesystem>
namespace Rndr
{
	void flipVertically(unsigned char* data, int width, int height, int channels) {
		int rowSize = width * channels;
		std::vector<unsigned char> tempRow(rowSize);

		for (int y = 0; y < height / 2; ++y) {
			unsigned char* topRow = data + y * rowSize;
			unsigned char* bottomRow = data + (height - y - 1) * rowSize;

			// Swap the top row with the bottom row
			std::memcpy(tempRow.data(), topRow, rowSize);
			std::memcpy(topRow, bottomRow, rowSize);
			std::memcpy(bottomRow, tempRow.data(), rowSize);
		}
	}
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		std::filesystem::path current_path = std::filesystem::current_path();
		RNDR_CORE_INFO("Current path: {0}", current_path.string());
		RNDR_CORE_INFO("Loading texture from path: {0}", path);
		auto inp = OIIO::ImageInput::open(path);
		if (!inp)
		{
			RNDR_CORE_ASSERT(inp, "Could not open image");
			RNDR_CORE_ERROR("Could not open image");
		}
		else
		{
			RNDR_CORE_INFO("Image opened successfully");
		}
		const OIIO::ImageSpec &spec = inp->spec();
		m_Width = spec.width;
		m_Height = spec.height;
		m_nChannels = spec.nchannels;
		auto pixels = std::make_unique<unsigned char[]>(m_Width * m_Height * m_nChannels);

		inp->read_image(OIIO::TypeDesc::UINT8, pixels.get());
		inp->close();
		RNDR_CORE_INFO("Number of channels: {0}", m_nChannels);
		RNDR_CORE_INFO("Image width: {0}, height: {1}", m_Width, m_Height);

		flipVertically(pixels.get(), m_Width, m_Height, m_nChannels);

		GLenum internalFormat = 0, dataFormat = 0;
		if (m_nChannels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (m_nChannels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		RNDR_CORE_ASSERT(internalFormat & dataFormat, "Format not supported");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, pixels.get());
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = 4;
		RNDR_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}