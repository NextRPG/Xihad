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
	class IParticleSystemLoaderEnv : public virtual irr::IReferenceCounted
	{
	public:	
		virtual ~IParticleSystemLoaderEnv() {}
		
		virtual void setPosition(irr::scene::ISceneNode* node, const irr::core::vector3df& pos) const = 0;
		virtual irr::core::vector3df getPosition(irr::scene::ISceneNode* node) const = 0;
		virtual irr::core::aabbox3df getAABB(irr::scene::ISceneNode* node) const = 0;
		virtual irr::scene::IMesh* getMesh(const char* meshDesc) const = 0;
		virtual irr::scene::ISceneNode* getNode(const char* nodeDesc) const = 0;
	};
}}

