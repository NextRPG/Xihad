#pragma once
#include <Dialogue\ITickMethod.hpp>

namespace xihad { namespace dialogue
{
	class IDialogue;
	class CDelayedTick : public ITickMethod
	{
	public:
		explicit CDelayedTick(IDialogue& target, ITickMethod& tickMethod, float delayedSeconds);
		virtual ~CDelayedTick();

		virtual void setTickSpeed( float speed ) override;

		virtual float getTickSpeed() const
		{

		}

		virtual void tick( float delta );

	protected:

	private:
		IDialogue& mTarget;
		ITickMethod& mTickMethod;
	};
}}

