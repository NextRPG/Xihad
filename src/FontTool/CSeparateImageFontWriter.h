#pragma once
#include "IFontWriter.h"
#include <string>

namespace irr { namespace video
{
	class IVideoDriver;
}}

namespace xihad { namespace font
{
	using namespace irr;
	class CSeparateImageFontWriter : public IFontWriter
	{
	public:
		CSeparateImageFontWriter(video::IVideoDriver* driver, const char* filename, const char* fmt);

		virtual ~CSeparateImageFontWriter();

		virtual bool write( const SFontInfo& );

		const std::string& getFilename() const { return filename; }
		const std::string& getFormat() const { return format; }

	protected:
		virtual std::string getImageName(u32 i);
		virtual bool onWriteFontFile(const SFontInfo&) = 0;

	private:
		std::string filename;
		std::string format;
		video::IVideoDriver* driver;
	};
}}

