#pragma once
#include "IParticleSystemLoaderEnv.h"
#include <irrlicht/smartptr.h>

namespace xihad { namespace particle
{
	class CDefaultEnv : public IParticleSystemLoaderEnv
	{
		typedef scene::ISceneNode Node;
		typedef scene::IMesh Mesh;

	public:
		CDefaultEnv(Node* source, Node* target);

		virtual void setPosition( Node* node, const core::vector3df& pos ) const;
		virtual core::vector3df getPosition( Node* node ) const;
		virtual core::aabbox3df getAABB( Node* node ) const;

		virtual Mesh* getMesh( const char* meshDesc ) const;
		virtual Node* getNode( const char* nodeDesc ) const;

	private:
		irrptr<Node> mSource;
		irrptr<Node> mTarget;
	};
}}

