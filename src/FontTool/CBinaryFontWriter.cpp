#include "CBinaryFontWriter.h"
#include <irrlicht\IrrlichtDevice.h>
#include <irrlicht\IFileSystem.h>
#include <irrlicht\IWriteFile.h>

namespace xihad { namespace font
{
	using namespace io;

	enum { FONT_VERSION = 1000, };

	template <typename T>
	inline static void writeT(IWriteFile* writer, const T& t)
	{
		writer->write(&t, sizeof(T));
	}

	CBinaryFontWriter::CBinaryFontWriter( IrrlichtDevice* device, const char* filename, const char* fmt, bool useAlpha ) :
		CSeparateImageFontWriter(device->getVideoDriver(), filename, fmt),
		device(device), useAlphaChannel(useAlpha)
	{
		device->grab();
	}

	CBinaryFontWriter::~CBinaryFontWriter()
	{
		device->drop();
	}

	bool CBinaryFontWriter::onWriteFontFile( const SFontInfo& fontInfo )
	{
		std::string filename = getFilename() + ".xft";
		IWriteFile* writer = device->getFileSystem()->createAndWriteFile(filename.c_str());

		writeT(writer, 0xabcdefff);
		writeT(writer, (int) FONT_VERSION);
		writeT(writer, useAlphaChannel);
		writeT(writer, fontInfo.images.size());
		std::string imageName;
		for (u32 i = 0; i < fontInfo.images.size(); ++i)
		{
			imageName = getImageName(i);

			// write image
			writeT(writer, imageName.size());
			writer->write(imageName.c_str(), imageName.size());
		}

		// write charmap
		writeT(writer, fontInfo.charMap.size());
		for (const std::pair<wchar_t, SFontArea>& chToarea : fontInfo.charMap)
			writeT(writer, chToarea);

		writer->drop();
		return true;
	}

}}

