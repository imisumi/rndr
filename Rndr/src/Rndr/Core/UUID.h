#pragma once

// #include <xhash>
#include <functional>
#include <cstdint>

namespace Rndr
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid)
			: m_UUID(uuid) {}

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<Rndr::UUID>
	{
		size_t operator()(const Rndr::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}