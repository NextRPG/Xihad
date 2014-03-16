#pragma once
#include "SFontInfo.h"
#include "IImageFactory.h"

namespace xihad { namespace font
{
	class IFontBitmapFactory
	{
	public:
		virtual ~IFontBitmapFactory() {}

		virtual void createFontInfo(SFontInfo& out, IImageFactory*) = 0;
	};
}}
