#include "CBaseDialogueBuilder.hpp"
#include <cassert>
#include "CDialogue.hpp"
#include "ITextContent.hpp"
#include "ITextElement.hpp"
#include "IDialogueContext.hpp"
#include "CAlignedTextSection.hpp"

namespace xihad { namespace dialogue
{
	CBaseDialogueBuilder::CBaseDialogueBuilder( 
		IDialogueContext* factory, unsigned widthLimit ) : 
		mWidthLimit(widthLimit), mFactory(factory), mSectionCount(0u)
	{
		assert(factory != 0);
	}

	CBaseDialogueBuilder::~CBaseDialogueBuilder()
	{
	}

	bool CBaseDialogueBuilder::newParagraph()
	{
		if (mParagraphs.empty() || !mParagraphs.back()->empty())
		{
			SParagraph* p = new SParagraph;
			mParagraphs.push_back(p);
			return true;
		}

		return false;
	}

	void CBaseDialogueBuilder::addText( ITextContent* text )
	{
		if (!text) return;

		if (mParagraphs.empty() && !newParagraph())
			throw std::exception("Paragraph creation failed!");

		mParagraphs.back()->push_back(text);
		++mSectionCount;
	}

	void CBaseDialogueBuilder::addTickEvent(ITickEvent* event)
	{
		if (!event) return;

		STickEvent te;
		te.sectionIndex = mSectionCount;
		te.event = event;
		mTickEvents.push_back(te);
	}

	IDialogue* CBaseDialogueBuilder::build()
	{
		// 1. Link text sections
		CAlignedTextSection* aligned = onLinkTextSections();
		
		// 2. Transfer text sections into text elements
		std::vector<irr_ptr<ITextElement>> elements;
		onCreateTextElements(aligned, elements);

		// 3. create dialogue from text elements
		IDialogue* d = onCreateDialogue(elements);

		// 4. reset building states
		reset();

		return d;
	}

	void CBaseDialogueBuilder::onCreateTextElements(
		CAlignedTextSection* aligned, 
		std::vector<irr_ptr<ITextElement>>& outElements)
	{
		ngn::position2di offset;
		while (aligned)
		{
			ITextElement* text = mFactory->create(aligned->getContent());
			text->setOffset(offset);
			offset += aligned->getOffset();
			outElements.push_back(text);

			aligned = aligned->getNextSection();
		}
	}


	void CBaseDialogueBuilder::reset()
	{
		mSectionCount = 0;
		for (SParagraph* p : mParagraphs)
			delete p;
		mParagraphs.clear();
		mTickEvents.clear();
	}

	IDialogue* CBaseDialogueBuilder::onCreateDialogue(
		std::vector<irr_ptr<ITextElement>>& elements)
	{
		return new CDialogue(elements, mTickEvents, mFactory.get());
	}

}}
