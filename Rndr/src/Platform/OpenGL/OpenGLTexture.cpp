#include "OpenGLTexture.h"

#include "Rndr/Log.h"

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

		GLenum format = (m_nChannels == 4) ? GL_RGBA : (m_nChannels == 3) ? GL_RGB : GL_RED;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, pixels.get());
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