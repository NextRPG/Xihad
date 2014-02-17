#pragma once
#include "IDialogueBuilder.hpp"
#include <list>

namespace xihad { namespace dialogue
{
	class ITextElement;
	class ITextElementFactory;
	class CBaseDialogueBuilder : public IDialogueBuilder
	{
	public:
		explicit CBaseDialogueBuilder(ITextElementFactory* factory, unsigned widthLimit);

		virtual void setWidthLimit(unsigned limit)
		{
			mWidthLimit = limit;
		}

		virtual unsigned getWidthLimit()
		{
			return mWidthLimit;
		}

		virtual void addParagraph(const STextSection& paragraph);
		virtual void addText(const STextSection& text);
		virtual void addTickEvent(const std::wstring& event);

	protected:
		ITextElement* createTextElement(const std::string& fontPath, const std::wstring& txt);

	private:
		struct SParagraph 
		{
			std::list<STextSection> texts;
		};

		unsigned mWidthLimit;
		ITextElementFactory* mFactory;
	};
}}

