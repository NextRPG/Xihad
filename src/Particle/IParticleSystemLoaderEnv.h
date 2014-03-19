#pragma once
#include <irrlicht/IReferenceCounted.h>
#include <irrlicht/vector3d.h>
#include <irrlicht/aabbox3d.h>

namespace irr { namespace scene 
{
	class IMesh;
	class ISceneNode;
}}

namespace xihad { namespace particle
{
	using namespace irr;
	class IParticleSystemLoaderEnv : public virtual IReferenceCounted
	{
	public:	
		virtual ~IParticleSystemLoaderEnv() {}
		
		virtual void deferMessage(scene::ISceneNode* msgSource, float delaySecs, const char* message) = 0;

		virtual void setPosition(scene::ISceneNode* node, const core::vector3df& pos) const = 0;
		virtual core::vector3df	getPosition(scene::ISceneNode* node) const = 0;
		virtual core::aabbox3df	getAABB(scene::ISceneNode* node) const = 0;
		virtual scene::IMesh*	getMesh(const char* meshDesc) const = 0;
		virtual scene::ISceneNode* getNode(const char* nodeDesc) const = 0;
	};
}}

