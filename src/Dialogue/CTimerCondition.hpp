#pragma once

namespace xihad { namespace dialogue
{
	class CTimerCondition
	{
	public:
		explicit CTimerCondition(float cycle = 0, int times = -1);

		bool satisfy(float delta);
		
		float getCycle() const
		{
			return mCycle;
		}

		void setCycle(float cycle);

		float getRatioInCycle() const
		{
			return mElapsedTime / mCycle;
		}

	private:
		float mCycle;
		float mElapsedTime;
		int mRestCycle;
	};
}}

