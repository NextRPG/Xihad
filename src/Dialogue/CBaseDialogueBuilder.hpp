#pragma once
#include "IDialogueBuilder.hpp"
#include "STickEvent.hpp"
#include <list>
#include <vector>
#include <Engine/irr_ptr.h>

namespace xihad { namespace dialogue
{
	class ITextElement;
	class ITextElementFactory;
	class CAlignedTextSection;
	class CBaseDialogueBuilder : public IDialogueBuilder
	{
	public:
		CBaseDialogueBuilder(ITextElementFactory* factory, unsigned widthLimit);

		virtual ~CBaseDialogueBuilder();

		virtual void setWidthLimit(unsigned limit) override
		{
			mWidthLimit = limit;
		}

		virtual unsigned getWidthLimit() override
		{
			return mWidthLimit;
		}

		virtual bool newParagraph() override;
		virtual void addText(const CTextSection& text) override;
		virtual void addTickEvent(ITickEvent* event) override;
		virtual IDialogue* build() override;

	protected:
		ITextElementFactory* getTextElementFactory() const
		{
			return mFactory.get();
		}

		virtual CAlignedTextSection* onLinkTextSections() = 0;

		virtual void onCreateTextElements(
			CAlignedTextSection* aligned,
			std::list<irr_ptr<ITextElement>>& outElements);

		virtual IDialogue* onCreateDialogue(
			std::list<irr_ptr<ITextElement>>& elements);

		virtual void reset();

	protected:
		typedef std::list<CTextSection> SParagraph;
		std::vector<SParagraph*> mParagraphs;
		std::list<STickEvent> mTickEvents;

	private:
		unsigned mWidthLimit;
		unsigned mSectionCount;
		irr_ptr<ITextElementFactory> mFactory;
	};
}}

