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
		typedef std::list<STickEvent> TickEvents;
		typedef SDialogueVisibility::ElementIter ElementIter;

	public:
		CDialogue(TextElements& elements, TickEvents& events, IDialogueContext* context);

		virtual ~CDialogue();

		virtual void setTickMethod(ITickMethod* method) override;
		
		virtual ITickMethod* getTickMethod() const override
		{
			return mTickMethod.get();
		}

		virtual void setEventEnabled(bool enable) override
		{
			mEventEnabled = enable;
		}

		virtual bool isEventEnabled()  const override
		{
			return mEventEnabled;
		}

		virtual IDialogueContext* getContext() const override
		{
			return mContext.get();
		}

		virtual void onUpdate(float deltaTime);

		virtual void setVisibility(const SDialogueVisibility& pVisibility, bool immediate) override;

		virtual SDialogueVisibility getVisibility()	  const override
		{
			return mVisibility;
		}

		virtual SDialogueVisibility beginVisibility() const override
		{
			return SDialogueVisibility(mTextElements.begin());
		}

		virtual SDialogueVisibility endVisibility()   const override
		{
			return SDialogueVisibility(mTextElements.end());
		}

	protected:
		virtual void triggerEvent(ElementIter bgn, ElementIter end, bool immediate);

	private:
		bool mEventEnabled;

		TickEvents mEvents;
		TextElements mTextElements;
		irr_ptr<ITickMethod> mTickMethod;
		SDialogueVisibility mVisibility;
		TickEvents::iterator mLastTriggerEvent;

		irr_ptr<IDialogueContext> mContext;
	};

}}

