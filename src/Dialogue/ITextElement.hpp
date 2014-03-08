#pragma once
#include <Engine/position2d.h>
#include <irrlicht/IReferenceCounted.h>

namespace xihad { namespace dialogue 
{
	class ITextAppearance;
	class ITextContent;
	class ITextElement : public virtual irr::IReferenceCounted
	{
	public:	
		virtual ~ITextElement() {}

		virtual void setVisible(unsigned bgnIndex, unsigned endIndex) = 0;
		virtual void setOffset(ngn::position2di pos) = 0;
		virtual const ITextContent* getContent() const = 0;
	};
}}

