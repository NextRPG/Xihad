#pragma once

namespace xihad { namespace render3d
{
	/// 动画剪辑数据
	/**
	 * @author etnlGD
	 * @date 2013年12月16日 14:25:56
	 */
	struct AnimationClipData
	{
		/// 剪辑开始帧
		int startFrame;

		/// 剪辑结束帧
		int endFrame;

		/// 动画播放速度，帧每秒
		float animationSpeed;

		AnimationClipData(int bgn, int end, float spd) : 
			startFrame(bgn), endFrame(end), animationSpeed(spd)
		{ }
	};
}}

