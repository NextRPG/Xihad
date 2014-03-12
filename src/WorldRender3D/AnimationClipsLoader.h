#pragma once
#include <CppBase\ReferenceCounted.h>
#include <string>
#include "AnimationClips.h"


namespace xihad { namespace render3d
{
	class AnimationClipsLoader : public ReferenceCounted
	{
	public:
		virtual AnimationClips load(const std::string& path) = 0;
	};
}}

