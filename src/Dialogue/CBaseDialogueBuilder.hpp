#pragma once
#include "IDialogueBuilder.hpp"
#include "STickEvent.hpp"
#include <list>
#include <vector>
#include <Engine/irr_ptr.h>

namespace xihad { namespace dialogue
{
	class ITextElement;
	class IDialogueContext;
	class CAlignedTextSection;
	class CBaseDialogueBuilder : public IDialogueBuilder
	{
	public:
		CBaseDialogueBuilder(IDialogueContext* factory, unsigned widthLimit);

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
		virtual void addText(ITextContent* text) override;
		virtual void addTickEvent(ITickEvent* event) override;
		virtual IDialogue* build() override;

	protected:
		IDialogueContext* getTextElementFactory() const
		{
			return mFactory.get();
		}

		virtual CAlignedTextSection* onLinkTextSections() = 0;

		virtual void onCreateTextElements(
			CAlignedTextSection* aligned,
			std::vector<irr_ptr<ITextElement>>& outElements);

		virtual IDialogue* onCreateDialogue(
			std::vector<irr_ptr<ITextElement>>& elements);

		virtual void reset();

	protected:
		typedef std::list<irr_ptr<ITextContent>> SParagraph;
		std::vector<SParagraph*> mParagraphs;
		std::list<STickEvent> mTickEvents;

	private:
		unsigned mWidthLimit;
		unsigned mSectionCount;
		irr_ptr<IDialogueContext> mFactory;
	};
}}

