#pragma once
#include "RenderComponent.h"

namespace irr { namespace scene
{
	class IMeshSceneNode;
	class IMesh;
}}

namespace xihad { namespace render3d
{
	class MeshManager;
	class MeshComponent : public RenderComponent
	{
	public:
		DEFINE_VISITABLE

		MeshComponent(const std::string& name, ngn::GameObject& host, IMeshSceneNode* node);

		static MeshComponent* create(
			const std::string& compName, ngn::GameObject& obj, const ngn::Properties& param,
			ISceneManager* smgr, MeshManager* msmgr);

		void setMesh(IMesh* mesh);

		IMesh* getMesh();

		virtual bool createTriangleSelector(SelectorInfo info = SelectorInfo());
		
	protected:
		IMeshSceneNode* getNode() const;
	};
}}

