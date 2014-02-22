#include "CAlignedDialogueBuilder.hpp"
#include "CDialogueAlignmenter.hpp"
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
			for (irr_ptr<ITextContent> visiting : *p)
			{
				int insertCount = mAlignmenter->insert(visiting.get());
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

