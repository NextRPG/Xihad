#pragma once
#include <unordered_map>
#include <Engine/xptr.h>
#include "AnimationClips.h"

namespace xihad { namespace render3d
{
	class AnimationClipsLoader;
	class AnimationClipsCache
	{
	public:
		AnimationClipsCache();

		~AnimationClipsCache();

		void setAnimationClipsLoader(AnimationClipsLoader*);

		AnimationClips getClips(const std::string& path);

		void addClips(const std::string& path, const AnimationClips& clip)
		{
			cachedClips[path] = clip;
		}

		void clearClips()
		{
			cachedClips.clear();
		}

	private:
		std::unordered_map<std::string, AnimationClips> cachedClips;
		xptr<AnimationClipsLoader> loader;
	};
}}

