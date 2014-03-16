#pragma once
#include "IFontWriter.h"
#include <irrlicht/irrString.h>

namespace irr {
	class IrrlichtDevice;
}

namespace xihad { namespace font
{
	class CXMLFontWriter : public IFontWriter
	{
	public:
		CXMLFontWriter(IrrlichtDevice* device, const char* filename, const char* fmt, bool useAlpha);

		virtual ~CXMLFontWriter();

		virtual bool write( const SFontInfo& );

	private:
		IrrlichtDevice* device;
		core::stringc filename;
		core::stringc format;
		bool UseAlphaChannel;
	};
}}

