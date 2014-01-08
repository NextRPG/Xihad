#pragma once

namespace luaT
{
	template <typename T1, typename T2>
	struct IsSameType
	{
		enum 
		{
			result = false,
		};
	};

	template <typename T>
	struct IsSameType<T, T>
	{
		enum 
		{
			result = true,
		};
	};
}

