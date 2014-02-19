#include "CAlignedTextSection.hpp"

namespace xihad { namespace dialogue 
{
	CAlignedTextSection::CAlignedTextSection( const CTextSection& base ) :
		CTextSection(base.getApperance(), base.getContent()), mNextSection(0)
	{

	}

	CAlignedTextSection::CAlignedTextSection( const ITextAppearance* apperance, ITextContent* content) :
		CTextSection(apperance, content), mNextSection(0)
	{
	}

	CAlignedTextSection* CAlignedTextSection::split( unsigned index )
	{
		CTextSection buffer;
		CTextSection::split(buffer, index);
		if (buffer.getContent() != nullptr)
			return new CAlignedTextSection(getApperance(), buffer.getContent());

		return 0;
	}

}}

