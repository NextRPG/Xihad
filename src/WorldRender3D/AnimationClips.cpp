#include "AnimationClips.h"
#include <boost/property_tree/ptree.hpp>
#include "AnimationClipData.h"
#include "CppBase/StdMap.h"
#include "CppBase/IteratorPair.h"

using namespace std;
using namespace boost;
using namespace property_tree;
namespace xihad { namespace render3d
{
	struct AnimationClips::impl
	{
		unordered_map<std::string, AnimationClipData> clipDataMap;
	};

	inline static bool validSpeed(float spd)
	{
		return spd != 0.f;
	}

	inline static float getDefaultSpeed(IteratorPair<const ptree>& clipIter)
	{
		// 保存指向第二个元素的迭代器，如果没有第三元素，那么就使用这个值
		auto iterWithValue = clipIter;

		if (clipIter.next() && clipIter.notEnd())
			iterWithValue = clipIter;

		float defaultAnimSpeed = iterWithValue->second.get<float>("");
		if (!validSpeed(defaultAnimSpeed))
			throw ptree_bad_data("speed != 0", path("default animation speed"));

		return defaultAnimSpeed;
	}

	/**
	 * @param [in] data 
	 * @param [out] bgnFrame
	 * @param [in, out] endFrame
	 */
	inline static void getFrame(const ptree& data, int& bgnFrame, int& endFrame)
	{
		IteratorPair<const ptree> frameIter(data);

		if (frameIter.reachEnd())
			throw ptree_bad_data("Array required", path("frame array"));

		int _1st = frameIter->second.get<int>("");
		frameIter.next();
		if (frameIter.reachEnd())
		{
			bgnFrame = endFrame + 1;
			endFrame = _1st;
		}
		else
		{
			bgnFrame = _1st;
			endFrame = frameIter->second.get<int>("");
		}
	}

	AnimationClips::AnimationClips() : mImpl(new impl)
	{
	}

	AnimationClips AnimationClips::loadFromPtree( const ptree& allClips )
	{
		typedef ptree::path_type path;

		AnimationClips clips;
		try
		{
			int bgnFrame, endFrame = 0;
			bool firstElement = true;
			float defaultAnimSpeed = sGlobalDefaultAnimSpeed;
			for(const ptree::value_type& clip : allClips)
			{
				IteratorPair<const ptree> clipIter(clip.second);

				if (clipIter.reachEnd())
					throw ptree_bad_path("no more data", path("clip_name"));
				string clipName = clipIter->second.get<string>("");

				if (clipIter.next() && clipIter.reachEnd())
					throw ptree_bad_path("no more data", path("frame/speed"));

#pragma region ReadDefaultAnimationSpeed
				if (firstElement)
				{
					firstElement = false;
					if (clipName == "default")
					{
						defaultAnimSpeed = getDefaultSpeed(clipIter);
						continue;
					}
				}
#pragma endregion

				getFrame(clipIter->second, bgnFrame, endFrame);

				float animationSpeed;
				if (clipIter.next() && clipIter.notEnd())
					animationSpeed = clipIter->second.get<float>("");
				else 
					animationSpeed = defaultAnimSpeed;

				AnimationClipData d(bgnFrame, endFrame, animationSpeed);
				clips.addClip(clipName, d);
			}  
		}
		catch (ptree_bad_data& e)
		{
			cerr << "bad tree data: " << e.what() << endl;
		}
		catch (ptree_bad_path& e)
		{
			cerr << "bad tree path: " << e.what() << endl;
		}

		
		return clips;
	}

	void AnimationClips::traverse( Visitor* visitor ) const
	{
		if (mImpl.get() == nullptr || visitor == nullptr) 
			return;

		for (auto& kv : mImpl->clipDataMap)
			visitor->onClip(kv.first, kv.second);
	}

	void AnimationClips::addClip( const std::string& clipname, const AnimationClipData& d )
	{
		ngn::StdMap::insert(mImpl->clipDataMap, clipname, d);
	}

	AnimationClipData* AnimationClips::getClip( const std::string& clipname )
	{
		auto got = mImpl->clipDataMap.find(clipname);

		if (got == mImpl->clipDataMap.end())
		{
			return nullptr;
		}
		else
		{
			return &got->second;
		}
	}

	float AnimationClips::getDefaultAnimationSpeed()
	{
		return sGlobalDefaultAnimSpeed;
	}

	float AnimationClips::setDefaultAnimationSpeed( float spd )
	{
		if (!validSpeed(spd))
			cerr << "try to set default speed to 0, it's illegal" << endl;
		else 
			sGlobalDefaultAnimSpeed = spd;

		return sGlobalDefaultAnimSpeed;
	}

	float AnimationClips::sGlobalDefaultAnimSpeed = 15;


}}

