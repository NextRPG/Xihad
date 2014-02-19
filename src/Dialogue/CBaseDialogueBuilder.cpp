#include "CBaseDialogueBuilder.hpp"
#include <cassert>
#include "CDialogue.hpp"
#include "ITextElement.hpp"
#include "ITextAppearance.hpp"
#include "ITextElementFactory.hpp"
#include "CAlignedTextSection.hpp"

namespace xihad { namespace dialogue
{
	CBaseDialogueBuilder::CBaseDialogueBuilder( 
		ITextElementFactory* factory, unsigned widthLimit ) : 
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

	void CBaseDialogueBuilder::addText( const CTextSection& text )
	{
		if (mParagraphs.empty() && !newParagraph())
			throw std::exception("Paragraph creation failed!");

		mParagraphs.back()->push_back(text);
		++mSectionCount;
	}

	void CBaseDialogueBuilder::addTickEvent(ITickEvent* event)
	{
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
		std::list<irr_ptr<ITextElement>> elements;
		onCreateTextElements(aligned, elements);

		// 3. create dialogue from text elements
		IDialogue* d = onCreateDialogue(elements);

		// 4. reset building states
		reset();

		return d;
	}

	void CBaseDialogueBuilder::onCreateTextElements(
		CAlignedTextSection* aligned, 
		std::list<irr_ptr<ITextElement>>& outElements)
	{
		ngn::position2di offset;
		while (aligned)
		{
			ITextElement* text = mFactory->create(aligned->getApperance(), aligned->getContent());
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
		std::list<irr_ptr<ITextElement>>& elements)
	{
		return new CDialogue(elements, mTickEvents);
	}

}}
