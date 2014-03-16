#include "CTextAlignmenter.h"
#include <irrlicht/irrMath.h>

namespace xihad { namespace font
{
	using namespace core;

	//! To avoid ugly lines between letters
	enum { MARGIN = 1 };

	CTextAlignmenter::CTextAlignmenter( const dimension2du& size ) :
		size(size), lineHeight(0) { }
	
	bool CTextAlignmenter::insertRect(const dimension2du& bbox, vector2di& outPos)
	{
		pen.X += MARGIN;
		int right = pen.X + bbox.Width;
		if (right < 0 || (u32)right < size.Width)
		{
			lineHeight = max_(lineHeight, bbox.Height);
		}
		else // Wrap line
		{
			pen.X = 0;
			pen.Y += (lineHeight+MARGIN);
			right = bbox.Width;
			lineHeight = bbox.Height;
		}


		bool filled = pen.Y + lineHeight > size.Height;
		if (filled)
		{
			pen = vector2di();
			lineHeight = bbox.Height;
		}

		outPos = pen;
		pen.X = right;
		return !filled;
	}

}}
