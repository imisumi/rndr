#pragma once

#include <unordered_map>

#include "opengl/FrameBuffer.h"

#include <string>

namespace Rndr
{
	class FrameBufferLibrary
	{
	public:
		// void Add(const std::string& name, const Ref<FrameBuffer>& frameBuffer);
		void Add(const std::string& name, const FrameBufferSpecification& spec);

		Ref<FrameBuffer> Get(const std::string& name);

		bool Exists(const std::string& name) const;



		// Iterator typedefs
		typedef typename std::unordered_map<std::string, Ref<FrameBuffer>>::iterator iterator;
		typedef typename std::unordered_map<std::string, Ref<FrameBuffer>>::const_iterator const_iterator;

		iterator begin() { return m_FrameBuffers.begin(); }
		iterator end() { return m_FrameBuffers.end(); }

		const_iterator begin() const { return m_FrameBuffers.begin(); }
		const_iterator end() const { return m_FrameBuffers.end(); }
	private:
		std::unordered_map<std::string, Ref<FrameBuffer>> m_FrameBuffers;
	};
}