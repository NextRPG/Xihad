#include "CXMLFontWriter.h"
#include <irrlicht/IrrlichtDevice.h>
#include <irrlicht/IFileSystem.h>
#include <irrlicht/IXMLWriter.h>

namespace xihad { namespace font
{
	using namespace video;

	CXMLFontWriter::CXMLFontWriter( IrrlichtDevice* device, 
		const char* filename, const char* fmt, bool useAlpha ) :
		device(device), filename(filename), format(fmt), UseAlphaChannel(useAlpha)
	{
		device->grab();
	}

	CXMLFontWriter::~CXMLFontWriter()
	{
		device->drop();
	}

	bool CXMLFontWriter::write( const SFontInfo& fontInfo )
	{
		const core::array<IImage*>& currentImages = fontInfo.images;
		const std::map<wchar_t, SFontArea>& CharMap = fontInfo.charMap;

		if (currentImages.size() == 0)
		{
			device->getLogger()->log("No image data to write, aborting.");
			return false;
		}

		core::stringc fn = filename;
		core::stringc imagename = filename;
		fn += ".xml";

		io::IXMLWriter *writer = device->getFileSystem()->createXMLWriter(fn.c_str());

		// header and line breaks
		writer->writeXMLHeader();
		writer->writeLineBreak();

		// write information
		writer->writeElement(L"font", false, L"type", L"bitmap");
		writer->writeLineBreak();
		writer->writeLineBreak();

		// write images and link to them
		for (u32 i=0; i<currentImages.size(); ++i)
		{
			imagename = filename;
			imagename += (s32)i;
			imagename += ".";
			imagename += format;
			device->getVideoDriver()->writeImageToFile(currentImages[i],imagename.c_str());

			writer->writeElement(L"Texture", true,
				L"index", core::stringw(i).c_str(),
				L"filename", core::stringw(imagename.c_str()).c_str(),
				L"hasAlpha", UseAlphaChannel ? L"true" : L"false");
						
			writer->writeLineBreak();
		}

		writer->writeLineBreak();

		// write each character
		std::map<wchar_t, SFontArea>::const_iterator it = CharMap.begin();
		while (it != CharMap.end())
		{
			const SFontArea &fa = it->second;

			wchar_t c[2];
			c[0] = it->first;
			c[1] = L'\0';
			core::stringw area, under, over, image;
			area  = core::stringw(fa.rectangle.UpperLeftCorner.X);
			area += L", ";
			area += fa.rectangle.UpperLeftCorner.Y;
			area += L", ";
			area += fa.rectangle.LowerRightCorner.X;
			area += L", ";
			area += fa.rectangle.LowerRightCorner.Y;

			core::array<core::stringw> names;
			core::array<core::stringw> values;
			names.clear();
			values.clear();
			// char
			names.push_back(core::stringw(L"c"));
			values.push_back(core::stringw(c));
			// image number
			if (fa.sourceimage != 0)
			{
				image = core::stringw(fa.sourceimage);
				names.push_back(core::stringw(L"i"));
				values.push_back(image);
			}
			// rectangle
			names.push_back(core::stringw(L"r"));
			values.push_back(area);

			if (fa.underhang != 0)
			{
				under = core::stringw(fa.underhang);
				names.push_back(core::stringw(L"u"));
				values.push_back(under);
			}
			if (fa.overhang != 0)
			{
				over = core::stringw(fa.overhang);
				names.push_back(core::stringw(L"o"));
				values.push_back(over);
			}
			writer->writeElement(L"c", true, names, values);

			writer->writeLineBreak();
			++it;
		}

		writer->writeClosingTag(L"font");

		writer->drop();

		device->getLogger()->log("Bitmap font saved.");
		return true;
	}

}}

