#include "BillboardComponent.h"
#include "irrlicht\IBillboardSceneNode.h"

using namespace std;
using namespace xihad::ngn;
using namespace irr;
using namespace scene;
using namespace video;
namespace xihad { namespace render3d
{
	BillboardComponent::BillboardComponent( const string& name, 
		GameObject& host, IBillboardSceneNode* node ) :
		RenderComponent(name, host, node)
	{
	}

	void BillboardComponent::setSize(const ngn::dimension2df& size)
	{
		getNode()->setSize(size);
	}

	void BillboardComponent::setSize( float height, float bottomEdgeWidth, 
		float topEdgeWidth )
	{
		getNode()->setSize(height, bottomEdgeWidth, topEdgeWidth);
	}

	void BillboardComponent::getSize( float& height, float& bottomEdgeWidth, float& topEdgeWidth ) const
	{
		getNode()->getSize(height, bottomEdgeWidth, topEdgeWidth);
	}

	void BillboardComponent::setColor( const irr::video::SColor& overallColor )
	{
		getNode()->setColor(overallColor);
	}

	void BillboardComponent::setColor( const irr::video::SColor& topColor, const irr::video::SColor& bottomColor )
	{
		getNode()->setColor(topColor, bottomColor);
	}

	void BillboardComponent::getColor( irr::video::SColor& topColor, irr::video::SColor& bottomColor ) const
	{
		getNode()->getColor(topColor, bottomColor);
	}

	irr::scene::IBillboardSceneNode* BillboardComponent::getNode() const
	{
		return (irr::scene::IBillboardSceneNode*) RenderComponent::getNode();
	}
}}

