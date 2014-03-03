#pragma once
#include "IParticleSystemLoaderEnv.h"
#include <irrlicht/smartptr.h>

namespace xihad { namespace particle
{
	class CParticleEditorEnv : public IParticleSystemLoaderEnv
	{
		typedef irr::scene::ISceneNode Node;
		typedef irr::scene::IMesh Mesh;

	public:
		CParticleEditorEnv(Node* source, Node* target);

		virtual void setPosition( Node* node, const irr::core::vector3df& pos ) const;
		virtual irr::core::vector3df getPosition( Node* node ) const;
		virtual irr::core::aabbox3df getAABB( Node* node ) const;

		virtual Mesh* getMesh( const char* meshDesc ) const;
		virtual Node* getNode( const char* nodeDesc ) const;

	private:
		irrptr<Node> mSource;
		irrptr<Node> mTarget;
	};
}}

