#pragma once
#include <Engine/position2d.h>
#include <Engine/irr_ptr.h>
#include <cassert>

namespace xihad { namespace dialogue 
{
	class ITextContent;
	class CAlignedTextSection
	{
	public:
		CAlignedTextSection(ITextContent* content = 0);

		~CAlignedTextSection();

		/// Set relative offset to previous text section
		void setOffset(ngn::position2di offset) 
		{
			this->mOffset = offset;
		}

		ngn::position2di getOffset() const
		{
			return mOffset;
		}

		ITextContent* getContent() const
		{
			return mContent.get();
		}

		CAlignedTextSection* getNextSection() const 
		{
			return mNextSection;
		}

		void link(CAlignedTextSection* next)
		{
			assert(!mNextSection);
			mNextSection = next;
		}

		CAlignedTextSection* split(unsigned index);

	private:
		irr_ptr<ITextContent> mContent;
		CAlignedTextSection* mNextSection;
		ngn::position2di mOffset;
	};
}}

