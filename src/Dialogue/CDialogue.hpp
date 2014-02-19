#pragma once
#include <vector>
#include <list>
#include "IDialogue.hpp"
#include "CTextSection.hpp"
#include "STickEvent.hpp"

namespace xihad { namespace dialogue 
{
	class ITextElement;
	class CDialogue : public IDialogue
	{
		typedef std::list<irr_ptr<ITextElement>> TextElements;
		typedef std::list<STickEvent> TickEvents;

	public:
		CDialogue(TextElements& elements, TickEvents& events);

		virtual ~CDialogue();

		virtual void setTickMethod(ITickMethod* method) override
		{
			mTickMethod = method;
		}

		virtual ITickMethod* getTickMethod() const override
		{
			return mTickMethod;
		}

		bool atEnd();

		bool atBegin();

	private:
		TickEvents mEvents;
		TextElements mTextElements;

		// TODO 记录当前显示位置
		TextElements::iterator mCurrentSection;
		std::wstring::iterator mCurrentChar;

		ITickMethod* mTickMethod;
	};



}}

