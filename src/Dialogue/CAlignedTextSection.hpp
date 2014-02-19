#pragma once
#include "CTextSection.hpp"
#include <Engine/position2d.h>

namespace xihad { namespace dialogue 
{
	class CAlignedTextSection : public CTextSection
	{
	public:
		CAlignedTextSection(const CTextSection& base);

		CAlignedTextSection(const ITextAppearance* apperance = 0, ITextContent* content = 0);

		/// Set relative offset to previous text section
		void setOffset(ngn::position2di offset) 
		{
			this->mOffset = offset;
		}

		ngn::position2di getOffset() const
		{
			return mOffset;
		}

		void link(CAlignedTextSection* next, ngn::position2di nextRelativeToThis)
		{
			assert(!mNextSection);
			mNextSection = next;
			next->setOffset(nextRelativeToThis);
		}

		CAlignedTextSection* getNextSection() const 
		{
			return mNextSection;
		}

		CAlignedTextSection* split(unsigned index);

	private:
		CAlignedTextSection* mNextSection;
		ngn::position2di mOffset;
	};
}}

