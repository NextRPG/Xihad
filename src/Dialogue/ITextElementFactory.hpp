#pragma once
#include <irrlicht/IReferenceCounted.h>

namespace xihad { namespace dialogue 
{
	class ITextElement;
	class ITextAppearance;
	class ITextContent;
	class ITextElementFactory : public virtual irr::IReferenceCounted
	{
	public:	
		virtual ~ITextElementFactory() {}

		virtual ITextElement* create(
			const ITextAppearance* appearance, ITextContent* content) = 0;
	};

}}

