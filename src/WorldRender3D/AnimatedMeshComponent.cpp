#include "AnimatedMeshComponent.h"
#include <irrlicht\IAnimatedMeshSceneNode.h>
#include <irrlicht\ISceneManager.h>
#include <Engine\Properties.h>
#include <CppBase\StdMap.h>
#include "AnimationClips.h"
#include "MeshManager.h"
#include "AnimationClipsCache.h"

using namespace std;
using namespace boost;
namespace xihad { namespace render3d
{
	using namespace ngn;

	AnimatedMeshComponent::AnimatedMeshComponent( 
		string const& name, GameObject& host, 
		IAnimatedMeshSceneNode* node ) :
		RenderComponent(name, host, node)
	{
	}

	AnimatedMeshComponent::~AnimatedMeshComponent()
	{
	}

	void AnimatedMeshComponent::setCurrentFrame( float frame )
	{
		getNode()->setCurrentFrame(frame);
	}

	bool AnimatedMeshComponent::setFrameLoop( int begin, int end )
	{
		return getNode()->setFrameLoop(begin, end);
	}

	void AnimatedMeshComponent::setTransitionTime( float Time )
	{
		getNode()->setTransitionTime(Time);
	}

	void AnimatedMeshComponent::setAnimationSpeed( float framesPerSecond )
	{
		getNode()->setAnimationSpeed(framesPerSecond);
	}

	float AnimatedMeshComponent::getAnimationSpeed() const
	{
		return getNode()->getAnimationSpeed();
	}

	IBoneSceneNode* AnimatedMeshComponent::getJointNode( const char* jointName )
	{
		return getNode()->getJointNode(jointName);
	}

	IBoneSceneNode* AnimatedMeshComponent::getJointNode( int jointID )
	{
		return getNode()->getJointNode(jointID);
	}

	int AnimatedMeshComponent::getJointCount() const
	{
		return getNode()->getJointCount();
	}

	float AnimatedMeshComponent::getFrameNumber() const
	{
		return getNode()->getFrameNr();
	}

	int AnimatedMeshComponent::getStartFrame() const
	{
		return getNode()->getStartFrame();
	}

	int AnimatedMeshComponent::getEndFrame() const
	{
		return getNode()->getEndFrame();
	}

	void AnimatedMeshComponent::setLoopMode( bool playAnimationLooped )
	{
		return getNode()->setLoopMode(playAnimationLooped);
	}

	bool AnimatedMeshComponent::getLoopMode() const
	{
		return getNode()->getLoopMode();
	}

	void AnimatedMeshComponent::setAnimationEndCallback( IAnimationEndCallBack* callback/*=0*/ )
	{
		return getNode()->setAnimationEndCallback(callback);
	}

	void AnimatedMeshComponent::setMesh( IAnimatedMesh* mesh )
	{
		return getNode()->setMesh(mesh);
	}

	IAnimatedMesh* AnimatedMeshComponent::getMesh()
	{
		return getNode()->getMesh();
	}

	bool AnimatedMeshComponent::createTriangleSelector( SelectorInfo info )
	{
		if (info.type == Stupid)
		{
			ISceneManager* smgr = getNode()->getSceneManager();
			ITriangleSelector* sel = smgr->createTriangleSelector(getNode());
			if (sel != nullptr)
			{
				getNode()->setTriangleSelector(sel);
				sel->drop();
				return true;
			}
		}
		else
		{
			return RenderComponent::createTriangleSelector(info);
		}

		return false;
	}

	void AnimatedMeshComponent::createClips( const AnimationClips& clips )
	{
		class AMC_visitor : public AnimationClips::Visitor
		{
		public:
			void onClip(const std::string& clipname, const AnimationClipData& clip) override
			{
				this_component->createClip(clipname, clip);
			}

			AnimatedMeshComponent* this_component;
		};

		AMC_visitor visitor;
		visitor.this_component = this;
		clips.traverse(&visitor);
	}

	void AnimatedMeshComponent::createClip( const std::string& clipName, const AnimationClipData& data )
	{
		StdMap::insert(mClips, clipName, data);
	}

	bool AnimatedMeshComponent::hasClip( const std::string& clipName ) const
	{
		return StdMap::containsKey(mClips, clipName);
	}

	void AnimatedMeshComponent::playAnimation( const std::string& clipName )
	{
		AnimationClipData* got = StdMap::findValuePtr(mClips, clipName);
		if (got != nullptr)
		{
			setFrameLoop(got->startFrame, got->endFrame);
			setAnimationSpeed(got->animationSpeed);
		}
		else
			cout << "No such clip: " << clipName << endl;
	}

	IShadowVolumeSceneNode* AnimatedMeshComponent::addShadowVolume( 
		const IMesh* shadowMesh, bool zfail, float infinity )
	{
		IShadowVolumeSceneNode* shadow = 
			getNode()->addShadowVolumeSceneNode(shadowMesh, -1, zfail, infinity);

		return shadow;
	}

	IAnimatedMeshSceneNode * AnimatedMeshComponent::getNode() const
	{
		return (IAnimatedMeshSceneNode*) RenderComponent::getNode();
	}

	AnimatedMeshComponent* AnimatedMeshComponent::create(
		const std::string& compName, ngn::GameObject& obj, const ngn::Properties& param,
		ISceneManager* smgr, AnimationClipsCache* clipsCache, MeshManager* meshManager)
	{
		IAnimatedMesh* mesh = nullptr;

		if (const char* path = param.getString("mesh"))
		{
			// TODO 未做任何检测，因为 Irrlicht 的 IMesh API 设计问题。
			// 将来引入自己的 MeshCache 来解决这一问题。
			mesh = static_cast<IAnimatedMesh*>(meshManager->getMesh(path));
		}

		AnimationClips clips;
		if (const char* path = param.getString("clips"))
			clips = clipsCache->getClips(path);

		vector3df zero, one(1, 1, 1);
		IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(
			mesh, 0, -1, zero, zero, one, true);

		auto amc = new AnimatedMeshComponent(compName, obj, node);

		amc->createClips(clips);

		return amc;
	}


}}

