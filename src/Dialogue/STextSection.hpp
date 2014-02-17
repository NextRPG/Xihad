#pragma once
#include <string>
#include <Engine/SColor.h>
#include <Engine/position2d.h>

namespace irr { namespace gui 
{
	class IGUIFont;
}}

namespace xihad { namespace dialogue 
{
	struct STextSection
	{
		irr::gui::IGUIFont* font;
		ngn::SColor fontColor;
		ngn::position2df offset;
		std::wstring content;
	};
}}

