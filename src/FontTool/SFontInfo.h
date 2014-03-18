#pragma once
#include <irrlicht/IGUIFontBuilder.h>
#include <irrlicht/irrArray.h>
#include <irrlicht/rect.h>
#include <map>

namespace irr { namespace video 
{
	class IImage;
}}

namespace xihad { namespace font
{
	using namespace irr;

	typedef gui::IGUIFontBuilder::SCharacterArea SFontArea;
// 	struct SFontArea
// 	{
// 		core::recti rectangle;
// 		s32			underhang;
// 		s32			overhang;
// 		u32			sourceimage;
// 	};

	struct SFontInfo
	{
		std::map<wchar_t, SFontArea>charMap;
		core::array<video::IImage*> images;
	};
}}

