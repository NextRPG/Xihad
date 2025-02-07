#include "CAlignedTextSection.hpp"
#include "ITextContent.hpp"

namespace xihad { namespace dialogue 
{
	CAlignedTextSection::CAlignedTextSection(ITextContent* content) :
		mContent(content), mNextSection(0)
	{
	}

	CAlignedTextSection::~CAlignedTextSection()
	{
	}

	CAlignedTextSection* CAlignedTextSection::split( unsigned index )
	{
		ITextContent* splitted = mContent->split(index);
		CAlignedTextSection* section = splitted ? new CAlignedTextSection(splitted) : nullptr;
		if (splitted) splitted->drop();
		return section; 
	}

}}

