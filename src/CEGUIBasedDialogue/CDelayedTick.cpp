#include "CDelayedTick.h"

namespace xihad { namespace dialogue
{
	CDelayedTick::CDelayedTick( IDialogue& target, ITickMethod* tickMethod, float delayedSeconds )
		: mTarget(target), mTickMethod(tickMethod), delayed(delayedSeconds)
	{

	}

	void CDelayedTick::setTickSpeed( float speed )
	{
		mTickMethod->setTickSpeed(speed);
	}

	float CDelayedTick::getTickSpeed() const
	{
		return mTickMethod->getTickSpeed();
	}

	void CDelayedTick::tick( float delta )
	{
		if (delayed > 0) 
			delayed -=delta;
		else
			mTickMethod->tick(delta);
	}

}}

