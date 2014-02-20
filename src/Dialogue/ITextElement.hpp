#pragma once
#include "ITextContent.hpp"
#include <Engine/position2d.h>

namespace xihad { namespace dialogue 
{
	class ITextAppearance;
	class ITextElement : public ITextContent
	{
	public:	
		virtual ~ITextElement() {}

		virtual void setVisible(unsigned bgnIndex, unsigned endIndex) = 0;
		virtual void setOffset(ngn::position2di pos) = 0;
	};
}}

