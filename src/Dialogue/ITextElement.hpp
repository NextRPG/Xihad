#pragma once
#include <irrlicht/IReferenceCounted.h>
#include <Engine/SColor.h>
#include <Engine/position2d.h>

namespace xihad { namespace dialogue 
{
	class ITextElement : public virtual irr::IReferenceCounted
	{
	public:	
		virtual ~ITextElement() {}

		virtual void setVisible(bool visible) = 0;
		virtual void setText(const wchar_t* txt) = 0;
		virtual void setColor(ngn::SColor color) = 0;
		virtual void setOffset(ngn::position2df pos) = 0;
	};
}}

