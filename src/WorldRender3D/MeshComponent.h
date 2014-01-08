#pragma once
#include "RenderComponent.h"
#include "irrlicht\IMeshSceneNode.h"

namespace xihad { namespace render3d
{
	class MeshComponent : public RenderComponent
	{
	public:
		DEFINE_VISITABLE

		MeshComponent(const std::string& name, ngn::GameObject& host, 
					  irr::scene::IMeshSceneNode* node);

		void setMesh(irr::scene::IMesh* mesh);

		irr::scene::IMesh* getMesh();

		virtual bool createTriangleSelector(SelectorInfo info = SelectorInfo());

	protected:
		irr::scene::IMeshSceneNode* getNode() const;
	};
}}

