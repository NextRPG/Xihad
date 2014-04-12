#include "CSeparateImageFontWriter.h"
#include <irrlicht/IVideoDriver.h>
#include <iostream>

using namespace std;
namespace xihad { namespace font
{
	CSeparateImageFontWriter::CSeparateImageFontWriter( 
		video::IVideoDriver* driver, const char* filename, const char* fmt ) :
		driver(driver), filename(filename), format(fmt)
	{
		driver->grab();
	}

	CSeparateImageFontWriter::~CSeparateImageFontWriter()
	{
		driver->drop();
	}

	bool CSeparateImageFontWriter::write( const SFontInfo& fnt )
	{
		if (fnt.images.size() == 0)
		{
			cerr << "No image data to write, aborting." << endl;
			return false;
		}

		for (u32 i = 0; i < fnt.images.size(); ++i)
		{
			std::string imageFileName = getImageName(i);
			const char* fname = imageFileName.c_str();
			if (!driver->writeImageToFile(fnt.images[i], fname))
			{
				cerr << "Writting image: " << fname << " failed" << endl;
				return false;
			}
			else
			{
				cout << fname << " saved" << endl;
			}
		}

		return onWriteFontFile(fnt);
	}

	std::string CSeparateImageFontWriter::getImageName( u32 i )
	{
		std::string imagename = filename;
		imagename += std::to_string(i);
		imagename += ".";
		imagename += format;
		return imagename;
	}

}}

