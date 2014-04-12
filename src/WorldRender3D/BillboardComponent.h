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

		BillboardComponent(const std::string& name, ngn::GameObject& host, IBillboardSceneNode* node );	


		static BillboardComponent* create(
			const std::string& name, ngn::GameObject& host, const ngn::Properties& params,
			ISceneManager* smgr);

		void setSize(const ngn::dimension2df& size);
		
		void setSize(float height, float bottomEdgeWidth, float topEdgeWidth);
		
		void getSize(float& height, float& bottomEdgeWidth, float& topEdgeWidth) const;

		void setColor(const video::SColor& overallColor);
		
		void setColor(const video::SColor& topColor, const video::SColor& bottomColor);
		
		void getColor(video::SColor& topColor, video::SColor& bottomColor) const;

	private:
		IBillboardSceneNode* getNode() const;
		
	};
}}

