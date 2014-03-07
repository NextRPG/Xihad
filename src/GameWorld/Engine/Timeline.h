#pragma once

namespace xihad { namespace ngn
{
	/// 时间线
	/**
	 * 在游戏主循环中记录全局时间，并且提供最近一次更新间隔。
	 *	- 可以对时间线 @link pause() 暂停@endlink ，暂停之后可以对之步进，方便调试。
	 *	- 可以对时间线 @link setTimeScale() 缩放@endlink ，缩放之后每次都会以正常速度的倍数进行更新
	 * 
	 * @author etnlGD
	 * @date 2013年12月14日 20:27:53
	 */
	class Timeline
	{
	public:
		/**
		 * @param initTimeSeconds reset()
		 * @param singleStep setSingleStepSeconds()
		 */
		explicit Timeline(double initTimeSeconds = 0.0) : 
			mElapsedSeconds(initTimeSeconds) {}

		/// 重置时间线
		/**
		 * 重置之后，最近更新间隔变为 0
		 * @see getElapsedSeconds() getLastTimeChange()
		 * @param initTimeSeconds 重置之后， getElapsedSeconds() 对应的值
		 */
		void reset(double initTimeSeconds)
		{
			mElapsedSeconds = initTimeSeconds;
			mDeltaSeconds = 0;
		}

		/// 更新时间线
		void update(float dtSeconds)
		{		
			mDeltaSeconds = dtSeconds;
			mElapsedSeconds += mDeltaSeconds;
		}

		/// 取得与另外一个时间线的时差
		double calcDeltaSeconds(const Timeline& other) const
		{
			return mElapsedSeconds - other.mElapsedSeconds;
		}

		/// 取得最近一次时间更新间隔
		float getLastTimeChange() const
		{
			return mDeltaSeconds;
		}

		/// 取得经过的所有时间
		double getElapsedSeconds() const 
		{
			return mElapsedSeconds;
		}

	private:
		double mElapsedSeconds;
		float mDeltaSeconds;
	};
}}

