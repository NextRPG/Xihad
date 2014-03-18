#pragma once

namespace irr { namespace gui
{
	class IGUIFontBuilder;
	class IGUIFontBitmap;
}}

namespace irr { namespace io
{
	class IReadFile;
}}

namespace xihad { namespace font
{
	using namespace irr;
	class CBinaryFontReader
	{
	public:
		virtual gui::IGUIFontBitmap* loadFont(io::IReadFile* reader, gui::IGUIFontBuilder*);
	};
}}

