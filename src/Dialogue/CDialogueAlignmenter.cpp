#include "CDialogueAlignmenter.hpp"
#include "CAlignedTextSection.hpp"
#include "ITextContent.hpp"
#include <cassert>

namespace xihad { namespace dialogue 
{
	CDialogueAlignmenter::CDialogueAlignmenter(unsigned widthLimit, ngn::dimension2di knl) :
		mWidthLimit(widthLimit), mKerningHeight(0),
		mCurrentLineHeight(0), mPrevWidth(0), mRelativeOffsetToPrev(knl.Width, 0), 
		mDialogueHead(nullptr), mCurrentLineHead(nullptr), mLastInserted(nullptr)
	{
		setKerningNewLine(knl);
	}

	unsigned CDialogueAlignmenter::getTotalHeight() const
	{
		return mOffset.Y + mCurrentLineHeight;
	}

	void CDialogueAlignmenter::setWidthLimit( unsigned widthLimit )
	{
		mWidthLimit = widthLimit;
		setKerningNewLine(mKerningNewLine);	// check knl
		if (mOffset.X > 0 && (unsigned)mOffset.X > mWidthLimit)
			wrapLine();
	}

	void CDialogueAlignmenter::newLine()
	{
		onLineFilled();
		wrapLine(true);
	}

	int CDialogueAlignmenter::insert(ITextContent* text)
	{
		if (text != nullptr && !text->empty()) 
			return insertNonEmptySection(new CAlignedTextSection(text));
		
		return 0; // Ignore empty text
	}

	int CDialogueAlignmenter::insertNonEmptySection(CAlignedTextSection* inserting)
	{
		int insertCount = 0;
		ITextContent::SFillResult res;
		while (inserting)
		{
			// compute alignment info for next section
			ITextContent* content = inserting->getContent();
			res = content->fillHorizontal(mWidthLimit-mOffset.X, mCurrentLineHead!=0);

			unsigned idx = res.splitIndex;
			if (idx > 0)	// ++insertCount?
			{
				++insertCount;
				advanceLine(res.width, res.height);
				linkNewSection(inserting);

				// update loop status
				if (idx >= content->endIndex())
					break;	// No more to insert in this pass
				else 
					inserting = inserting->split(idx);	// insert the rest
			}

			onLineFilled();
			wrapLine();
		}
		mRelativeOffsetToPrev.X = res.width;
		mRelativeOffsetToPrev.Y = 0;

		return insertCount;
	}

	void CDialogueAlignmenter::linkNewSection( CAlignedTextSection* newSection )
	{
		if (mLastInserted)
			mLastInserted->link(newSection);

		newSection->setOffset(mRelativeOffsetToPrev);
		mLastInserted = newSection;

		if (!mCurrentLineHead)
		{
			mCurrentLineHead = newSection;	// Validate line head

			if (!mDialogueHead)
				mDialogueHead = newSection;
		}
	}

	void CDialogueAlignmenter::onLineFilled()
	{
		// We can implement Left-Align, Center-Align, and Right-Align here
	}

	void CDialogueAlignmenter::reset()
	{
		mPrevWidth = 0;
		mCurrentLineHeight = 0;
		mRelativeOffsetToPrev = ngn::position2di();
		mOffset = ngn::position2di();
		mCurrentLineHead = nullptr;
		mLastInserted = nullptr;

		while (mDialogueHead)
		{
			CAlignedTextSection* buff = mDialogueHead->getNextSection();
			delete mDialogueHead;
			mDialogueHead = buff;
		}
		assert(mDialogueHead == nullptr);

		setKerningNewLine(mKerningNewLine);
	}

	void CDialogueAlignmenter::setKerningNewLine( ngn::dimension2di knl )
	{
		std::swap(mKerningNewLine, knl);
		checkKNL();

		if (mCurrentLineHead == 0)
		{
			int dx = (mKerningNewLine.Width - knl.Width);
			int dy = (mKerningNewLine.Height - knl.Height);
			mOffset.X += dx;
			mOffset.Y += dy;
			mRelativeOffsetToPrev.X += dx;
			mRelativeOffsetToPrev.Y += dy;

			mPrevWidth = knl.Width;
		}
	}

}}
