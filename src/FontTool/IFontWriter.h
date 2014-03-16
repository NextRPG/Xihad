#pragma once
#include "SFontInfo.h"

namespace xihad { namespace font
{
	class IFontWriter
	{
	public:	
		virtual ~IFontWriter() {}
		virtual bool write(const SFontInfo&) = 0;
	};
}}

