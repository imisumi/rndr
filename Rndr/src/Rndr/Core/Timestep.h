#pragma once

namespace Rndr
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		operator float() const { return m_Time; }
		// operator bool() const { return m_Time > 0.0f; }
		// operator for float * timestep
		// float operator*(float other) const { return m_Time * other; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}