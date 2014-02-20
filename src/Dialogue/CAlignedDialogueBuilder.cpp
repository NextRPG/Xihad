#include "CAlignedDialogueBuilder.hpp"
#include "CDialogueAlignmenter.hpp"
#include "ITextSectionSplitter.hpp"
#include "IDialogueContext.hpp"
#include "ITextAppearance.hpp"
#include "ITextContent.hpp"
#include "CTickEventRedirection.hpp"

namespace xihad { namespace dialogue
{
	CAlignedDialogueBuilder::CAlignedDialogueBuilder(
		IDialogueContext* factory, CDialogueAlignmenter* alignmenter ) :
		CBaseDialogueBuilder(factory, alignmenter->getWidthLimit()), 
		mAlignmenter(alignmenter)
	{
	}

	CAlignedDialogueBuilder::~CAlignedDialogueBuilder()
	{
	}

	CAlignedTextSection* CAlignedDialogueBuilder::onLinkTextSections()
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

	void CAlignedDialogueBuilder::reset()
	{
		mAlignmenter.reset();
		CBaseDialogueBuilder::reset();
	}

}}

