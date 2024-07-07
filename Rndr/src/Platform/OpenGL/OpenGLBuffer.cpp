#include "OpenGLBuffer.h"

#include <glad/glad.h>


#include "ImGui/backends/imgui_impl_opengl3.h"

namespace Rndr
{

	//? Vertex Buffer
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

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

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
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
		? Frame Buffer --------------------------------------------------------------------------------------------------
	*/

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
		: m_Specification(spec)
	{

		// print the width and height of the framebuffer
		RNDR_CORE_INFO("Framebuffer width = {0}, height = {1}", m_Specification.Width, m_Specification.Height);
		// Invalidate();
		glCreateFramebuffers(1, &m_RendererID);

		// Create a texture and attach it to the m_RendererID
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_ColorAttachment, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_ColorAttachment, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureStorage2D(m_ColorAttachment, 1, GL_RGBA8, m_Specification.Width, m_Specification.Height); // Allocate storage for the m_ColorAttachment

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

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		{
			RNDR_CORE_WARN("Attempted to resize framebuffer to the same size {0}, {1}", width, heigth);
			return;
		}
		RNDR_CORE_INFO("Resizing framebuffer from {0}, {1} to {2}, {3}", m_Specification.Width, m_Specification.Height, width, heigth);

		
		m_Specification.Width = width;
		m_Specification.Height = heigth;

		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);

		glCreateFramebuffers(1, &m_RendererID);

		// Create a texture and attach it to the m_RendererID
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_ColorAttachment, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_ColorAttachment, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_ColorAttachment, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureStorage2D(m_ColorAttachment, 1, GL_RGBA8, m_Specification.Width, m_Specification.Height); // Allocate storage for the m_ColorAttachment

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

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//TODO: idk why but if I don't call this function, the ImGui will not render the UI
		ImGui_ImplOpenGL3_CreateFontsTexture();
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