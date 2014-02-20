#pragma once
#include <string>
#include <irrlicht/IReferenceCounted.h>
#include <Engine/SColor.h>
#include <Engine/position2d.h>

namespace xihad { namespace dialogue 
{
	class ITextAppearance;
	class ITextElement : public virtual irr::IReferenceCounted
	{
	public:	
		virtual ~ITextElement() {}

		virtual void setVisible(bool visible) = 0;
		virtual void setContent(const std::wstring& content) = 0;
		virtual void setOffset(ngn::position2di pos) = 0;
	};
}}

