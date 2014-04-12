#pragma once
#include <irrlicht/vector2d.h>
#include <unordered_set>

typedef irr::core::vector2di Vec2;

namespace std {
	template <> struct std::hash<Vec2>
	{
		size_t operator()(const Vec2& _Keyval) const
		{
			return (_Keyval.Y * 1021) + _Keyval.X;
		}
	};
}
