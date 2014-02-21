#pragma once

namespace xihad { namespace dialogue
{
	class IDialogue;
	class ITickEvent;
	class ITextContent;
	class IDialogueBuilder
	{
	public:
		virtual ~IDialogueBuilder() {}

		virtual void setWidthLimit(unsigned limit) = 0;
		virtual unsigned getWidthLimit() = 0;

		virtual bool newParagraph() = 0;
		virtual void addText(ITextContent* text) = 0;
		virtual void addTickEvent(ITickEvent* event) = 0;

		virtual IDialogue* build() = 0;
	};
}}

