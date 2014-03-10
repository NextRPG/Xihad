#include "AnimatedMeshComponent.h"
#include <irrlicht\IAnimatedMeshSceneNode.h>
#include "irrlicht\ISceneManager.h"
#include "CppBase\StdMap.h"
#include "AnimationClips.h"

using namespace std;
using namespace xihad::ngn;
using namespace irr;
using namespace scene;
namespace xihad { namespace render3d
{
	
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

	irr::scene::IBoneSceneNode* AnimatedMeshComponent::getJointNode( const char* jointName )
	{
		return getNode()->getJointNode(jointName);
	}

	irr::scene::IBoneSceneNode* AnimatedMeshComponent::getJointNode( int jointID )
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

	void AnimatedMeshComponent::setAnimationEndCallback( irr::scene::IAnimationEndCallBack* callback/*=0*/ )
	{
		return getNode()->setAnimationEndCallback(callback);
	}

	void AnimatedMeshComponent::setMesh( irr::scene::IAnimatedMesh* mesh )
	{
		return getNode()->setMesh(mesh);
	}

	irr::scene::IAnimatedMesh* AnimatedMeshComponent::getMesh()
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

	irr::scene::IAnimatedMeshSceneNode * AnimatedMeshComponent::getNode() const
	{
		return (irr::scene::IAnimatedMeshSceneNode*) RenderComponent::getNode();
	}


}}

