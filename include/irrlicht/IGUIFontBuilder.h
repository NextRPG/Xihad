#pragma once
#include "IReferenceCounted.h"
#include "rect.h"
#include "path.h"

namespace irr { namespace video
{
	class ITexture;
}}
namespace irr { namespace gui
{
	class IGUIFontBitmap;
	class IGUIFontBuilder : public IReferenceCounted
	{
	public:
		struct SCharacterArea
		{
			core::recti rectangle;
			s32			underhang;
			s32			overhang;
			u32			sourceimage;
		};

	public:	
		virtual void addCharArea(wchar_t, const SCharacterArea&) = 0;

		virtual bool addTexture(const io::path& imagePath, bool useAlphaChannel) = 0;

		virtual bool addTexture(video::ITexture* texture) = 0;

		virtual IGUIFontBitmap* build() = 0;
	};
}}	

