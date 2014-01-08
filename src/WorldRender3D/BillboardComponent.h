#pragma once
#include "RenderComponent.h"
#include "Engine\dimension2d.h"
#include "irrlicht\SColor.h"

namespace irr { namespace scene
{
	class IBillboardSceneNode;
}}

namespace xihad { namespace render3d
{
	class BillboardComponent : public RenderComponent
	{
	public:
		DEFINE_VISITABLE

		BillboardComponent(	const std::string& name, ngn::GameObject& host, 
							irr::scene::IBillboardSceneNode* node );	

		void setSize(const ngn::dimension2df& size);
		
		void setSize(float height, float bottomEdgeWidth, float topEdgeWidth);
		
		void getSize(float& height, float& bottomEdgeWidth, float& topEdgeWidth) const;

		void setColor(const irr::video::SColor& overallColor);
		
		void setColor(const irr::video::SColor& topColor,
			const irr::video::SColor& bottomColor);
		
		void getColor(irr::video::SColor& topColor,
			irr::video::SColor& bottomColor) const;

	private:
		irr::scene::IBillboardSceneNode* getNode() const;
		
	};
}}

