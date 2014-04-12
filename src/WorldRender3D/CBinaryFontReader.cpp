#include "CBinaryFontReader.h"
#include <FontTool/SFontInfo.h>
#include <irrlicht/irrTypes.h>
#include <irrlicht/IReadFile.h>
#include <irrlicht/IGUIFontBuilder.h>
#include <utility>
#include <iostream>

namespace xihad { namespace font
{
	using namespace gui;
	using namespace io;

	template <typename T>
	T readT(IReadFile* reader)
	{
		T data;
		if (reader->read(&data, sizeof(T)) < sizeof(T))
			throw std::exception("Can't read T from file");

		return data;
	}

	template <typename T>
	T* readArrayT(IReadFile* reader, u32 size)
	{
		T* array = new T[size];
		s32 bytes = (s32)(sizeof(T)*size);

		if (reader->read(array, bytes) < bytes)
		{
			delete[] array;
			throw std::exception("Can't read array of T from file");
		}

		return array;
	}

	path readString(IReadFile* reader, u32 len)
	{
		char* imageName = new char[len+1];
		
		if (reader->read(imageName, len) < (s32)len)
		{
			delete[] imageName;
			throw std::exception("Can't read string from file");
		}

		imageName[len] = '\0';

		path imagePath = imageName;
		delete[] imageName;

		return imagePath;
	}

	enum { SUPPORT_FONT_VERSION = 1000, };
	IGUIFontBitmap* CBinaryFontReader::loadFont( IReadFile* reader, IGUIFontBuilder* fb )
	{
		try
		{
			// Magic number check
			if (readT<int>(reader) != 0xabcdefff)
			{
				std::cerr << "The given file is not [Xihad Font File]" << std::endl;
				return 0;
			}

			if (readT<int>(reader) != SUPPORT_FONT_VERSION)
			{
				std::cerr << "The font version is not supported" << std::endl;
				return 0;
			}

			bool useAlpha = readT<bool>(reader);
			u32 imageCount = readT<u32>(reader);
			for (u32 i = 0; i < imageCount; ++i)
			{
				u32 imageNameLength = readT<u32>(reader);
				path imageName = readString(reader, imageNameLength);
				if (!fb->addTexture(imageName, useAlpha))
					return 0;
			}

			u32 charMapSize = readT<u32>(reader);
			auto charMap = readArrayT<std::pair<wchar_t, SFontArea>>(reader, charMapSize);
			for (u32 i = 0; i < charMapSize; ++i)
				fb->addCharArea(charMap[i].first, charMap[i].second);
			delete[] charMap;
		}
		catch (std::exception&)
		{
			return 0;
		}
		
		return fb->build();
	}

}}

