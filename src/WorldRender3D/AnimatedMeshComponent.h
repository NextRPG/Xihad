#pragma once
#include "RenderComponent.h"
#include <unordered_map>
#include "AnimationClipData.h"

namespace irr { namespace scene
{
	class IShadowVolumeSceneNode;
	class IAnimatedMeshSceneNode;
	class IBoneSceneNode;
	class IAnimationEndCallBack;
	class IAnimatedMesh;
	class IMesh;
}}

namespace xihad { namespace render3d
{
	class MeshManager;
	class AnimationClips;
	class AnimationClipsCache;
	class AnimatedMeshComponent : public RenderComponent
	{
	public:
		DEFINE_VISITABLE

		AnimatedMeshComponent(std::string const& name, ngn::GameObject& host, 
							  IAnimatedMeshSceneNode* node);

		static AnimatedMeshComponent* create(
			const std::string& compName, ngn::GameObject& obj, const ngn::Properties& param,
			ISceneManager*, AnimationClipsCache*, MeshManager*);

		void createClips(const AnimationClips& clips);
		void createClip(const std::string& clipName, const AnimationClipData& data);
		bool hasClip(const std::string& clipName) const;
		void playAnimation(const std::string& clipName);

		void setCurrentFrame(float frame);

		bool setFrameLoop(int begin, int end);

		void setTransitionTime(float Time);

		void setAnimationSpeed(float framesPerSecond);
		float getAnimationSpeed() const;

		IBoneSceneNode* getJointNode(const char* jointName);
		IBoneSceneNode* getJointNode(int jointID);
		int getJointCount() const;

		//! Returns the currently displayed frame number.
		float getFrameNumber() const;

		//! Returns the current start frame number.
		int getStartFrame() const;

		//! Returns the current end frame number.
		int getEndFrame() const;

		//! Sets looping mode which is on by default.
		/** If set to false, animations will not be played looped. */
		void setLoopMode(bool playAnimationLooped);

		//! returns the current loop mode
		/** When true the animations are played looped */
		bool getLoopMode() const;

		//! Sets a callback interface which will be called if an animation playback has ended.
		/** Set this to 0 to disable the callback again.
		Please note that this will only be called when in non looped
		mode, see IAnimatedMeshSceneNode::setLoopMode(). */
		void setAnimationEndCallback(IAnimationEndCallBack* callback=0);

		//! Sets a new mesh
		void setMesh(IAnimatedMesh* mesh);

		//! Returns the current mesh
		IAnimatedMesh* getMesh();

		virtual bool createTriangleSelector(SelectorInfo info = SelectorInfo());

		IShadowVolumeSceneNode* addShadowVolume(
			const IMesh* shadowMesh = nullptr, 
			bool zfail = true, float infinity = 1000.f);

	protected:
		virtual ~AnimatedMeshComponent();

	private:
		IAnimatedMeshSceneNode * getNode() const;

	private:
		std::unordered_map<std::string, AnimationClipData> mClips;
	};
}}

