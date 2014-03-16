#pragma once
#include "IFontWriter.h"

namespace xihad { namespace font
{
	class CBinaryFontWriter : public IFontWriter
	{
	public:
		virtual bool write( const SFontInfo& );

	};
}}

