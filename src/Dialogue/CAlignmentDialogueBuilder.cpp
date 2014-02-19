#include "CAlignmentDialogueBuilder.hpp"
#include "CDialogueAlignmenter.hpp"
#include "ITextSectionSplitter.hpp"
#include "ITextElementFactory.hpp"
#include "ITextAppearance.hpp"
#include "ITextContent.hpp"
#include "CTickEventRedirection.hpp"

namespace xihad { namespace dialogue
{
	CAlignmentDialogueBuilder::CAlignmentDialogueBuilder(
		ITextElementFactory* factory, CDialogueAlignmenter* alignmenter ) :
		CBaseDialogueBuilder(factory, alignmenter->getWidthLimit()), 
		mAlignmenter(alignmenter)
	{
	}

	CAlignmentDialogueBuilder::~CAlignmentDialogueBuilder()
	{
	}

	CAlignedTextSection* CAlignmentDialogueBuilder::onLinkTextSections()
	{
		// helper to redirect section index of mTickEvents
		CTickEventRedirection<std::list<STickEvent>> eventRedirect(mTickEvents);
		for (SParagraph* p : mParagraphs)
		{
			for (CTextSection& visiting : *p)
			{
				int insertCount = mAlignmenter->insert(visiting);
				eventRedirect.onInsertion(insertCount);
			}

			mAlignmenter->newLine();
		}

		return mAlignmenter->getDialogueHead();
	}

	void CAlignmentDialogueBuilder::reset()
	{
		mAlignmenter.reset();
		CBaseDialogueBuilder::reset();
	}

}}

