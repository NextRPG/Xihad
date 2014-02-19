#include "CTextSection.hpp"
#include "ITextAppearance.hpp"
#include "ITextContent.hpp"

namespace xihad { namespace dialogue 
{
	CTextSection::CTextSection( const ITextAppearance* apperance, ITextContent* content ) :
		mContent(content)
	{
		setApperance(apperance);
	}

	CTextSection::~CTextSection()
	{
	}

	void CTextSection::setApperance( const ITextAppearance* newApperance )
	{
		mApperance = newApperance;
	}

	void CTextSection::split( CTextSection& rest, unsigned index )
	{
		if (mContent != nullptr) 
			rest.mContent = mContent->split(index);
	}

}}

