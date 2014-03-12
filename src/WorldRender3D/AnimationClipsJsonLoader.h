#pragma once
#include "AnimationClipsLoader.h"

namespace xihad { namespace render3d
{
	class AnimationClipsJsonLoader : public AnimationClipsLoader
	{
	public:
		virtual AnimationClips load(const std::string& path);
	};
}}

