#pragma once
#include <irrlicht/irrString.h>
#include "CSeparateImageFontWriter.h"

namespace irr {
	class IrrlichtDevice;
}

namespace xihad { namespace font
{
	class CXMLFontWriter : public CSeparateImageFontWriter
	{
	public:
		CXMLFontWriter(IrrlichtDevice* device, const char* filename, const char* fmt, bool useAlpha);

		virtual ~CXMLFontWriter();

	protected:
		virtual bool onWriteFontFile( const SFontInfo& );

	private:
		IrrlichtDevice* device;
		bool useAlphaChannel;
	};
}}

