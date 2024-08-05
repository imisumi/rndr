


#pragma once

#include <Rndr/Core/Base.h>

#include <vector>

namespace Rndr
{
	enum class FrameBufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RGBA16F,
		RGBA32F,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format)
			: TextureFormat(format) {}

		FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
		// TODO: filtering/wrap
	};

	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FrameBufferTextureSpecification> Attachments;
	};

	struct FrameBufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		FrameBufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		FrameBuffer(const FrameBufferSpecification& spec);
		~FrameBuffer();
		const FrameBufferSpecification& GetSpecification() const { return m_Specification; }


		void Invalidate();

		uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const;
		uint32_t GetDepthAttachmentRendererID() const { return m_DepthAttachment; }

		void Resize(uint32_t width, uint32_t height);
		int ReadPixel(uint32_t attachmentIndex, int x, int y);

		void ClearAttachment(uint32_t attachmentIndex, int value);

		uint32_t GetWidth() const { return m_Specification.Width; }
		uint32_t GetHeight() const { return m_Specification.Height; }

		void Bind();
		void Unbind();




		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	
	private:
		// uint32_t m_RendererID = 0;
		// uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		// FrameBufferSpecification m_Specification;


		uint32_t m_RendererID = 0;
		FrameBufferSpecification m_Specification;

		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecifications;
		FrameBufferTextureSpecification m_DepthAttachmentSpecification = FrameBufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}