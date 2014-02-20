#pragma once
#include <list>
#include "IDialogue.hpp"
#include "STickEvent.hpp"
#include <Engine/irr_ptr.h>

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
			return mTickMethod.get();
		}

		virtual void onUpdate(float deltaTime);

	private:
		TickEvents mEvents;
		TextElements mTextElements;
		irr_ptr<ITickMethod> mTickMethod;
	};



}}

