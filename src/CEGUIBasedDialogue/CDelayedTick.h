#pragma once
#include <Dialogue\ITickMethod.hpp>
#include <Engine\irr_ptr.h>

namespace xihad { namespace dialogue
{
	class IDialogue;
	class CDelayedTick : public ITickMethod
	{
	public:
		CDelayedTick(IDialogue& target, ITickMethod* tickMethod, float delayedSeconds);

		virtual ~CDelayedTick() {}

		virtual void setTickSpeed( float speed ) override;

		virtual float getTickSpeed() const;

		virtual void tick( float delta );

	private:
		IDialogue& mTarget;
		irr_ptr<ITickMethod> mTickMethod;
		float delayed;
	};
}}

