#pragma once
#include <Engine/dimension2d.h>
#include <irrlicht/IReferenceCounted.h>
#include <utility>

namespace xihad { namespace dialogue 
{
	class ITextContent;
	class IMetrics : public virtual irr::IReferenceCounted
	{
	public:
		struct SFillResult
		{
			int width;
			int height;
			unsigned splitIndex;

			SFillResult(unsigned splitIndex, int w = 0, int h = 0) :
				splitIndex(splitIndex), width(w), height(h)
			{
			}
		};

	public:	
		virtual ~IMetrics() {}

		virtual ngn::dimension2du getDimension(const wchar_t* text) const = 0;

		virtual SFillResult fillHorizontal(
			ITextContent* text, unsigned widthLimit, bool allowFillEmpty = true) const;
	};
}}

