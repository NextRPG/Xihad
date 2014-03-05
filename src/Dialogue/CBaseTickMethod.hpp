#pragma once
#include "ITickMethod.hpp"
#include "CTimerCondition.hpp"

namespace xihad { namespace dialogue
{
	class CBaseTickMethod : public ITickMethod
	{
	public:
		explicit CBaseTickMethod(float standardCycle, float initSpeed = 1.0f);

		virtual void setTickSpeed(float speed) override;
		virtual float getTickSpeed() const override;

		virtual void tick(float delta) override;

	protected:
		virtual bool onTick() = 0;

	private:
		float mStandardCycle;
		CTimerCondition mTimer;
	};
}}

