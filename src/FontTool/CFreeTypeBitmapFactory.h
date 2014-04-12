#pragma once
#include "IFontBitmapFactory.h"
#include <freetype/ft2build.h>
#include <freetype/freetype.h>

namespace xihad { namespace font
{
	class CFreeTypeBitmapFactory : public IFontBitmapFactory
	{
	public:
		explicit CFreeTypeBitmapFactory(IImageFactory::ColorFormat fmt, bool useAlpha);

		virtual ~CFreeTypeBitmapFactory();

		virtual void createFontInfo( SFontInfo& out, IImageFactory* );

		void setFontPath(const char* fontPath);

		void setCharSize(const core::dimension2du& d);

		void setImageSize(const core::dimension2du& d) { imageSize = d; }

	private:
		FT_Library library;
		FT_Face face;
		core::dimension2du imageSize;
		core::dimension2du charSize;
		IImageFactory::ColorFormat format;
		bool useAlphaChannel;
	};
}}

