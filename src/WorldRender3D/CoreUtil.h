pragma once
#include <irrlicht/vector3d.h>

namespace xihad { namespace srpg
{
	class CoreUtil
	{
	public:
		template <typename T>
		static irr::core::vector3d<T> fromArray(const T data[3])
		{
			return irr::core::vector3d(data[0], data[1], data[2]);
		}
	};
}}

