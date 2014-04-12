#pragma once

namespace xihad { namespace ngn
{
	class System
	{
	public:
		static double getCurrentTime();
		static double getSystemFrequency() { return frequency; }

	private:
		static double frequency;
	};
}}

