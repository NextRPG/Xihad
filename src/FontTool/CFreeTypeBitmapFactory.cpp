#include "CFreeTypeBitmapFactory.h"
#include <exception>
#include "CTextAlignmenter.h"
#include "CBitmapBuffer.h"

namespace xihad { namespace font
{
	using namespace video;
	using namespace core;
	using namespace std;

	inline static int fp2int(FT_Pos p)
	{
		return p >> 6;
	}

	inline static void newImage(SFontInfo& out, CBitmapBuffer& buffer, IImageFactory* factory, int height = -1)
	{
		IImage* img = buffer.createImage(factory, height);
		out.images.push_back(img);
	}

	inline static SFontArea getArea( FT_Glyph_Metrics metrics, u32 imgIdx, 
		const vector2di& pos, const dimension2du& size )
	{
		SFontArea area;
		area.underhang = fp2int(metrics.horiBearingX);
		area.overhang  = fp2int(metrics.horiAdvance - (metrics.width + metrics.horiBearingX));
		area.rectangle.UpperLeftCorner = pos;
		area.rectangle.LowerRightCorner.X = pos.X + size.Width;
		area.rectangle.LowerRightCorner.Y = pos.Y + size.Height;
		area.sourceimage = imgIdx;

		return area;
	}

	enum { SUCCESS_CODE = 0, };

/* ----------------------------------------------------------------------------*/
	CFreeTypeBitmapFactory::CFreeTypeBitmapFactory(IImageFactory::ColorFormat fmt, bool useAlpha) : 
		face(0), imageSize(256, 256), charSize(12, 12), 
		format(fmt), useAlphaChannel(useAlpha)
	{
		if (FT_Init_FreeType(&library) != SUCCESS_CODE)
			throw exception("Library init failed");
	}

	CFreeTypeBitmapFactory::~CFreeTypeBitmapFactory()
	{
		if (face)	 FT_Done_Face(face);
		if (library) FT_Done_FreeType(library);
	}

	void CFreeTypeBitmapFactory::setFontPath( const char* fontPath )
	{
		if (face) FT_Done_Face(face);

		if (FT_New_Face(library, fontPath, 0, &face) != SUCCESS_CODE)
			throw exception("Coundln't create face from specified font path");
	}

	void CFreeTypeBitmapFactory::setCharSize( const dimension2du& fontSize )
	{
		FT_Set_Pixel_Sizes(face, fontSize.Width, fontSize.Height);
	}

	void CFreeTypeBitmapFactory::createFontInfo( SFontInfo& out, IImageFactory* imgFactory )
	{
		CTextAlignmenter alignmenter(imageSize);
		CBitmapBuffer bitmapBuffer(imageSize, format, useAlphaChannel);

		FT_GlyphSlot slot  = face->glyph;
		FT_UInt glyphIndex = 0;
		FT_ULong charCode = FT_Get_First_Char(face, &glyphIndex);
		while (glyphIndex) // loop for all glyphs
		{
			if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_RENDER) != SUCCESS_CODE)
				throw exception("Glyph load failed");

			vector2di pen;
			dimension2du bbox(slot->bitmap.width, slot->bitmap.rows);
			if (!alignmenter.insertRect(bbox, pen))
			{
				newImage(out, bitmapBuffer, imgFactory);
				bitmapBuffer.clearBuffer();
			}
			bitmapBuffer.copyRect(pen, bbox, slot->bitmap.buffer);

			SFontArea area = getArea(slot->metrics, out.images.size(), pen, bbox);
			out.charMap[(wchar_t) charCode] = area;

			charCode = FT_Get_Next_Char(face, charCode, &glyphIndex);
		}

		// The last image
		if (alignmenter.getBelow())
			newImage(out, bitmapBuffer, imgFactory, alignmenter.getBelow());
	}

	

}}

