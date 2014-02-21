#include "CDialogue.hpp"
#include "ITextElement.hpp"
#include "ITickMethod.hpp"
#include "IDialogueContext.hpp"
#include <algorithm>

namespace xihad { namespace dialogue 
{
	CDialogue::CDialogue( TextElements& elements, 
		TickEvents& events, IDialogueContext* context ) :
		mEventEnabled(true), mContext(context)
	{
		mEvents.splice(mEvents.end(), events);
		mEvents.push_front(STickEvent());	// sentinel event will never be dereference
		mLastTriggerEvent = mEvents.begin();

		mTextElements.swap(elements);
	}

	CDialogue::~CDialogue()
	{
	}

	void CDialogue::setTickMethod( ITickMethod* method )
	{
		mTickMethod = method;
	}

	void CDialogue::onUpdate( float deltaTime )
	{
		mTickMethod->tick(deltaTime);
	}

	static void makeAllVisible(irr_ptr<ITextElement> element)
	{
		element->setVisible(0, element->endIndex());
	}

	static void makeAllInvisible(irr_ptr<ITextElement> element)
	{
		element->setVisible(0, 0);
	}

	void CDialogue::setVisibility(const SDialogueVisibility& pVisibility, bool immediate)
	{
		auto bgn = mVisibility.endElement;
		auto end = pVisibility.endElement;
		auto fnc = makeAllVisible;
		if (bgn < end)
		{
			if (bgn > mTextElements.begin()) --bgn;
			--end;
			// fnc = makeAllVisible;
		}
		else if (bgn > end)
		{
			std::swap(bgn, end);
			fnc = makeAllInvisible;
		}
		std::for_each(end, bgn, fnc);

		if (pVisibility.endElement != mTextElements.begin())
		{
			auto back = pVisibility.endElement;
			--back;
			(*back)->setVisible(0, pVisibility.endLetter);
		}
		else
		{
			assert(pVisibility.endLetter == 0);
		}

		if (mEventEnabled && bgn != end)
			triggerEvent(bgn, end, immediate);

		mVisibility = pVisibility;
	}

	void CDialogue::triggerEvent(ElementIter bgn, ElementIter end, bool immediate)
	{
		// bool mode;
		assert(bgn != end);

		unsigned idxBgn = bgn - mTextElements.begin();
		unsigned idxEnd = end - mTextElements.begin();
		assert(idxEnd > idxBgn);

		///< Try tail first
		TickEvents::iterator it = mLastTriggerEvent;
		++it;		// Eliminate the previous triggered event
		while (it != mEvents.end() && it->inRange(idxBgn, idxEnd))
		{
			it->event->onEventTrigger(this, immediate);
			++it;
		}

		if (--it != mLastTriggerEvent)
		{
			mLastTriggerEvent = it;
			return;	// There is some a event in tail triggered, just return
		}

		assert(it == mLastTriggerEvent);
		///< Try head instead because no event in tail triggered
		if (it != mEvents.begin())
		{
			--it;	// Eliminate the previous triggered event
			while (it != mEvents.begin() && (it->inRange(idxBgn, idxEnd)))
			{
				it->event->onEventTrigger(this, immediate);
				--it;
			}
			mLastTriggerEvent = ++it;
		}
	}

}}
