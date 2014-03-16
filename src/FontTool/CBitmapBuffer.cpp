#include "CBitmapBuffer.h"
#include <exception>
#include <assert.h>

namespace xihad { namespace font
{
	using namespace video;

	CBitmapBuffer::CBitmapBuffer(const core::dimension2du& size, ColorFormat fmt, bool alpha) :
		imageSize(size), format(fmt), useAlpha(alpha)
	{
		data = new unsigned char[imageSize.Width*imageSize.Height*getUnitSize()];
		clearBuffer();
	}

	CBitmapBuffer::~CBitmapBuffer()
	{
		delete[] data;
	}

	void CBitmapBuffer::copyRect(const core::vector2di& pos, const core::dimension2du& size, unsigned char* grayImageData)
	{
		assert(pos.X>=0 && pos.Y>=0 && 
			(pos.X+size.Width)<imageSize.Width && 
			(pos.Y+size.Height)<imageSize.Height);

		for (u32 srcX = (u32)pos.X, dstX = 0; dstX < size.Width; srcX++, dstX++ )
			for (u32 dstY = (u32)pos.Y, srcY = 0; srcY < size.Height; dstY++, srcY++ )
				setColor(srcX, dstY, grayImageData[srcY * size.Width + dstX]);
	}

	void CBitmapBuffer::clearBuffer()
	{
		for (u32 i = 0; i < imageSize.Width; i++)
			for (u32 j = 0; j < imageSize.Height; j++)
				setColor(i, j, 0);
	}

	unsigned char* CBitmapBuffer::pointer(int i, int j)
	{
		return &data[(j*imageSize.Width + i)*getUnitSize()];
	}

	void CBitmapBuffer::setColor( int i, int j, unsigned char gray)
	{
		unsigned char* dst = pointer(i, j);

		if (format == ECF_A8R8G8B8)
		{
			if (useAlpha)
			{
				dst[3] = gray;
				dst[0] = dst[1] = dst[2] = 255;
			}
			else
			{
				dst[3] = 255;
				dst[0] = dst[1] = dst[2] = gray;
			}
		}
		else if (format == ECF_A1R5G5B5)
		{
			short* p = (short*) dst;
			short gray_5b = gray >> 3;

			// set r/g/b channel
			*p = (gray_5b<<10) | (gray_5b<<5) | gray_5b;

			// set alpha channel
			if (gray == 0 && useAlpha)	// transparent?
				*p &= 0x7fff;			// set alpha to 0
			else 
				*p |= 0x8000;			// set alpha to 1
		}
		else
		{
			raiseUnsupportedColorFormat();	
		}
	}

	int CBitmapBuffer::getUnitSize() const
	{
		switch (format)
		{
		case ECF_A1R5G5B5:
			return 2;
		case ECF_R5G6B5:
			return 2;
		case ECF_R8G8B8:
			return 3;
		case ECF_A8R8G8B8:
			return 4;
		case ECF_R16F:
			return 2;	
		case ECF_G16R16F:
			return 4;	
		case ECF_A16B16G16R16F:
			return 8;
		case ECF_R32F:
			return 4;
		case ECF_G32R32F:
			return 8;
		case ECF_A32B32G32R32F:
			return 16;
		default:
			raiseUnsupportedColorFormat();
			return 0;
		}
	}

	void CBitmapBuffer::raiseUnsupportedColorFormat() const
	{
		throw std::exception("Unsupported color format");
	}

}}

