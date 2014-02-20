#pragma once
#include <irrlicht/IReferenceCounted.h>

namespace xihad { namespace dialogue 
{
	class ITextElement;
	class ITextAppearance;
	class ITextContent;
	class IDialogueContext : public virtual irr::IReferenceCounted
	{
	public:	
		virtual ~IDialogueContext() {}

		virtual ITextElement* create(ITextContent* content) = 0;
	};

}}

