#pragma once

#include <xhash>

namespace Rndr
{
	class UUID
	{
	public:
		UUID();

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