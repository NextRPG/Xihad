#include "AnimationClipsCache.h"
#include <CppBase/StdMap.h>
#include "AnimationClipsLoader.h"

namespace xihad { namespace render3d
{

	AnimationClipsCache::AnimationClipsCache() {}

	AnimationClipsCache::~AnimationClipsCache() {}

	void AnimationClipsCache::setAnimationClipsLoader( AnimationClipsLoader* newloader )
	{
		loader = newloader;
	}

	AnimationClips AnimationClipsCache::getClips( const std::string& path )
	{
		if (AnimationClips* loaded = StdMap::findValuePtr(cachedClips, path))
			return *loaded;

		AnimationClips clips;
		if (loader)
		{
			clips = loader->load(path);
			cachedClips[path] = clips;
		}

		return clips;
	}

}}
