#pragma once
#include "IParticleSystemLoaderEnv.h"
#include <irrlicht/smartptr.h>

namespace xihad { namespace particle
{
	class CDefaultEnv : public IParticleSystemLoaderEnv
	{
	protected:
		typedef scene::ISceneNode Node;
		typedef scene::IMesh Mesh;

	public:
		CDefaultEnv(Node* source, Node* target);
		
		virtual ~CDefaultEnv();

		virtual core::vector3df getPosition( Node* node ) const;
		virtual core::aabbox3df getAABB( Node* node ) const;

		virtual Mesh* getMesh( const char* meshDesc ) const;
		virtual Node* getNode( const char* nodeDesc ) const;

	private:
		irrptr<Node> mSource;
		irrptr<Node> mTarget;
	};
}}

