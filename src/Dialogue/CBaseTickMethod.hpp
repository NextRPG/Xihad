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
		virtual float getTickSpeed() override;

		virtual void setEventEnabled(bool enable) override;
		virtual bool isEventEnabled()  override;

		virtual void tick(float delta) override;

	protected:
		virtual bool onTick() = 0;
		virtual void triggerEvent();

	private:
		bool mEventEnabled;
		float mStandardCycle;
		CTimerCondition mTimer;
	};
}}

