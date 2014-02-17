#pragma once
#include <string>
#include "STextSection.hpp"

namespace xihad { namespace dialogue
{
	class IDialogue;
	class IDialogueBuilder
	{
	public:
		virtual ~IDialogueBuilder() {}

		virtual void setWidthLimit(unsigned limit) = 0;
		virtual unsigned getWidthLimit() = 0;

		virtual void addParagraph(const STextSection& paragraph) = 0;
		virtual void addText(const STextSection& text) = 0;
		virtual void addTickEvent(const std::wstring& event) = 0;

		virtual IDialogue* build() = 0;
	};
}}

