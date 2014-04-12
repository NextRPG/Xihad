#pragma once
#include <irrlicht/SColor.h>
#include <irrlicht/dimension2d.h>

namespace irr { namespace video 
{
	class IImage;
}}

namespace xihad { namespace font
{
	using namespace irr;

	class IImageFactory
	{
	public:
		typedef video::ECOLOR_FORMAT ColorFormat;

	public:	
		virtual ~IImageFactory() {}

		virtual video::IImage* createImage(
			ColorFormat format, core::dimension2du dimension, void* data) = 0;
	};
}}

