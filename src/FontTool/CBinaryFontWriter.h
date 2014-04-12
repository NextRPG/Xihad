#pragma once
#include "CSeparateImageFontWriter.h"

namespace irr {
	class IrrlichtDevice;
}

namespace xihad { namespace font
{
	class CBinaryFontWriter : public CSeparateImageFontWriter
	{
	public:
		CBinaryFontWriter(IrrlichtDevice* device, const char* filename, const char* fmt, bool useAlpha);

		virtual ~CBinaryFontWriter();

	protected:
		virtual bool onWriteFontFile( const SFontInfo& );

	private:
		IrrlichtDevice* device;
		bool useAlphaChannel;
	};
}}

