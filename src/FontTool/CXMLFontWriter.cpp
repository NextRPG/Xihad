#include "CXMLFontWriter.h"
#include <irrlicht/IrrlichtDevice.h>
#include <irrlicht/IFileSystem.h>
#include <irrlicht/IXMLWriter.h>

namespace xihad { namespace font
{
	using namespace video;

	CXMLFontWriter::CXMLFontWriter( IrrlichtDevice* device, const char* filename, const char* fmt, bool useAlpha ) :
		CSeparateImageFontWriter(device->getVideoDriver(), filename, fmt), 
		device(device), useAlphaChannel(useAlpha)
	{
		device->grab();
	}

	CXMLFontWriter::~CXMLFontWriter()
	{
		device->drop();
	}

	bool CXMLFontWriter::onWriteFontFile( const SFontInfo& fontInfo )
	{
		std::string fn = getFilename() + ".xml";
		io::IXMLWriter *writer = device->getFileSystem()->createXMLWriter(fn.c_str());

		// header and line breaks
		writer->writeXMLHeader();	
		writer->writeLineBreak();

		// write information
		writer->writeElement(L"font", false, L"type", L"bitmap");
		writer->writeLineBreak();
		writer->writeLineBreak();

		// write images and link to them
		for (u32 i=0; i < fontInfo.images.size(); ++i)
		{
			writer->writeElement(L"Texture", true,
				L"index", core::stringw(i).c_str(),
				L"filename", core::stringw(getImageName(i).c_str()).c_str(),
				L"hasAlpha", useAlphaChannel ? L"true" : L"false");
						
			writer->writeLineBreak();
		}

		writer->writeLineBreak();

		// write each character
		for (auto& it : fontInfo.charMap)
		{
			const SFontArea &fa = it.second;

			wchar_t c[2];
			c[0] = it.first;
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
		}

		writer->writeClosingTag(L"font");

		writer->drop();
		return true;
	}

}}

