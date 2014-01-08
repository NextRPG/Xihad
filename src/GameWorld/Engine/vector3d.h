#pragma once
#include <irrlicht/vector3d.h>

namespace xihad { namespace ngn
{
	typedef irr::core::vector3df vector3df;
	typedef irr::core::vector3di vector3di;

	/// vector3d 辅助函数集合
	struct vector3_helper
	{
		/// 从数组构造 vector3d
		/**
		 * @param data 三个元素的数组
		 * @return x=data[0], y=data[1], z=data[2]
		 */
		template <typename T>
		inline static irr::core::vector3d<T> fromArray(const T data[3])
		{
			return irr::core::vector3df(data[0], data[1], data[2]);
		}

		/// 从数组设置 vector3d
		/**
		 * vector.X=data[0], vector.Y=data[1], vector.Z=data[2], 
		 * @param vector 希望被设置的向量
		 * @param data 三个元素的数组
		 */
		template <typename T>
		inline static void setArray(irr::core::vector3d<T>& vector, const T data[3])
		{
			vector.set(data[0], data[1], data[2]);
		}
	};
}}

