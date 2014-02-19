#pragma once

namespace xihad { namespace dialogue 
{
	class CTextSection;
	class ITextSectionSplitter
	{
	public:	
		virtual ~ITextSectionSplitter() {}

		virtual CTextSection& split(CTextSection& source, unsigned index) = 0;
	};
}}

