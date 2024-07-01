#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Rndr
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//? Index Buffer


	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}




	/*
		? Frame Buffer
	*/

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
		: m_Specification(spec)
	{
		// Invalidate();
		glCreateFramebuffers(1, &m_RendererID);

		// Create a texture and attach it to the m_RendererID
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glTextureStorage2D(m_ColorAttachment, 1, GL_RGBA8, m_Specification.Width, m_Specification.Height); // Allocate storage for the m_ColorAttachment
		glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Attach the m_ColorAttachment to the m_RendererID
		glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0, m_ColorAttachment, 0);

		// Optional: Create and attach a renderbuffer for depth and stencil attachments
		glCreateRenderbuffers(1, &m_DepthAttachment);
		glNamedRenderbufferStorage(m_DepthAttachment, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);

		// Attach the renderbuffer to the m_RendererID
		glNamedFramebufferRenderbuffer(m_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment);

		// Check the completeness of the m_RendererID
		if (glCheckNamedFramebufferStatus(m_RendererID, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			RNDR_CORE_ERROR("Framebuffer is incomplete!");
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t heigth)
	{
		if (width == 0 || heigth == 0 || width > 16384 || heigth > 16384)
		{
			RNDR_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, heigth);
			return;
		}
		if (m_Specification.Width == width && m_Specification.Height == heigth)
			return;
		
		m_Specification.Width = width;
		m_Specification.Height = heigth;


		//TODO: check why this is not working
		// if (m_ColorAttachment)
		// 	glDeleteTextures(1, &m_ColorAttachment);
		// if (m_DepthAttachment)
		// 	glDeleteTextures(1, &m_DepthAttachment);

		// if (m_RendererID)
		// {
		// 	glDeleteFramebuffers(1, &m_RendererID);
		// 	glDeleteTextures(1, &m_ColorAttachment);
		// 	glDeleteTextures(1, &m_DepthAttachment);
		// }

		// glCreateFramebuffers(1, &m_RendererID);

		// Create a texture and attach it to the m_RendererID
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glTextureStorage2D(m_ColorAttachment, 1, GL_RGBA8, m_Specification.Width, m_Specification.Height); // Allocate storage for the m_ColorAttachment
		glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Attach the m_ColorAttachment to the m_RendererID
		glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0, m_ColorAttachment, 0);

		// Optional: Create and attach a renderbuffer for depth and stencil attachments
		glCreateRenderbuffers(1, &m_DepthAttachment);
		glNamedRenderbufferStorage(m_DepthAttachment, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);

		// Attach the renderbuffer to the m_RendererID
		glNamedFramebufferRenderbuffer(m_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment);

		// Check the completeness of the m_RendererID
		if (glCheckNamedFramebufferStatus(m_RendererID, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			RNDR_CORE_ERROR("Framebuffer is incomplete!");
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}