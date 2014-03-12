#include "BillboardComponent.h"
#include <irrlicht\IBillboardSceneNode.h>
#include <irrlicht\ISceneManager.h>

using namespace std;
namespace xihad { namespace render3d
{
	using namespace video;
	using namespace ngn;

	BillboardComponent::BillboardComponent( const string& name, 
		GameObject& host, IBillboardSceneNode* node ) :
		RenderComponent(name, host, node)
	{
	}

	BillboardComponent* BillboardComponent::create( 
		const std::string& name, ngn::GameObject& host, const ngn::Properties& params, 
		ISceneManager* smgr )
	{
		IBillboardSceneNode* node = smgr->addBillboardSceneNode();
		return new BillboardComponent(name, host, node);
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

	void BillboardComponent::setColor( const video::SColor& overallColor )
	{
		getNode()->setColor(overallColor);
	}

	void BillboardComponent::setColor( const video::SColor& topColor, const video::SColor& bottomColor )
	{
		getNode()->setColor(topColor, bottomColor);
	}

	void BillboardComponent::getColor( video::SColor& topColor, video::SColor& bottomColor ) const
	{
		getNode()->getColor(topColor, bottomColor);
	}

	IBillboardSceneNode* BillboardComponent::getNode() const
	{
		return (IBillboardSceneNode*) RenderComponent::getNode();
	}
}}

