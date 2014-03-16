#pragma once
#include <irrlicht/dimension2d.h>
#include <irrlicht/vector2d.h>
#include <irrlicht/irrTypes.h>

namespace xihad { namespace font
{
	using namespace irr;
	class CTextAlignmenter
	{
	public:
		CTextAlignmenter(const core::dimension2du& size);

		bool insertRect(const core::dimension2du& bbox, core::vector2di& outPos);

		int getBelow() const { return pen.Y + lineHeight; }

	private:
		core::dimension2du size;
		core::vector2di pen;
		u32 lineHeight;
	};
}}

