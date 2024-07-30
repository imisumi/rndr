
#include "FrameBufferLibrary.h"


namespace Rndr
{
	void FrameBufferLibrary::Add(const std::string& name, const FrameBufferSpecification& spec)
	{
		RNDR_CORE_ASSERT(!Exists(name), "Framebuffer already exists!");
		Ref<FrameBuffer> framebuffer = FrameBuffer::Create(spec);
		m_FrameBuffers[name] = framebuffer;
	}

	Ref<FrameBuffer> FrameBufferLibrary::Get(const std::string& name)
	{
		RNDR_CORE_ASSERT(Exists(name), "Framebuffer not found!");
		return m_FrameBuffers[name];
	}

	bool FrameBufferLibrary::Exists(const std::string& name) const
	{
		return m_FrameBuffers.find(name) != m_FrameBuffers.end();
	}
}