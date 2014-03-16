#pragma once
#include "IImageFactory.h"
#include <irrlicht/dimension2d.h>
#include <irrlicht/SColor.h>
#include <irrlicht/rect.h>

namespace xihad { namespace font
{
	using namespace irr;
	class CBitmapBuffer
	{
	public:
		typedef IImageFactory::ColorFormat ColorFormat;

	public:
		explicit CBitmapBuffer(const core::dimension2du& size, ColorFormat fmt, bool useAlpha);

		~CBitmapBuffer();

		void copyRect(const core::vector2di& pos, const core::dimension2du& size, unsigned char* grayImageData);

		void clearBuffer();

		video::IImage* createImage(IImageFactory* factory, int height = -1)
		{
			if (height < 0 || (u32) height > imageSize.Height)
				height = imageSize.Height;

			core::dimension2du dimen(imageSize.Width, height);
			return factory->createImage(format, dimen, data);
		}

	private:
		unsigned char* pointer(int i, int j);
		void setColor(int i, int j, unsigned char gray);
		int getUnitSize() const;
		void raiseUnsupportedColorFormat() const;

	private:
		CBitmapBuffer(const CBitmapBuffer&);
		CBitmapBuffer& operator=(const CBitmapBuffer&);

	private:
		unsigned char* data;
		const core::dimension2du imageSize;
		const ColorFormat format;
		const bool useAlpha;
	};
}}

