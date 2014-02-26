#include "RenderComponent.h"
#include "irrlicht/ISceneNode.h"
#include "irrlicht/ISceneManager.h"
#include "irrlicht/IVideoDriver.h"
#include "Engine/GameObject.h"
#include "Engine/Transform.h"
#include "CppBase/BitOperation.h"
#include "boost/cast.hpp"

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;
using namespace boost;
namespace xihad { namespace render3d
{
	using namespace ngn;
	using ::std::string;

	// fwd
	static void setUserdata2Node(irr::scene::ISceneNode* node, void* ud);

	RenderComponent::RenderComponent(const string& name, GameObject& host, ISceneNode* node ) :
		Component(name, host), mNode(node), mVisibility(1)
	{
		xassert(mNode);
		mNode->setParent(nullptr);

		setUserdata2Node(node, this);
		xassert(RenderComponent::getComponentFromNode(node) == this && 
				"Node named pointer error");
	}

	RenderComponent::~RenderComponent()
	{
		setUserdata2Node(mNode.get(), nullptr);
		xassert(RenderComponent::getComponentFromNode(mNode.get()) == nullptr);
	}

	const RenderComponent::AABB& RenderComponent::getBoundingBox() const
	{
		return mNode->getBoundingBox();
	}

	const RenderComponent::AABB RenderComponent::getTransformedBoundingBox() const
	{
		return mNode->getTransformedBoundingBox();
	}

	RenderComponent::Material& RenderComponent::getMaterial( irr::u32 num )
	{
		return mNode->getMaterial(num);
	}

	irr::u32 RenderComponent::getMaterialCount() const
	{
		return mNode->getMaterialCount();
	}

	void RenderComponent::setMaterialFlag( irr::video::E_MATERIAL_FLAG flag, bool newValue )
	{
		mNode->setMaterialFlag(flag, newValue);
	}

	void RenderComponent::setMaterialTexture( irr::u32 textureLayer, const irr::c8* texName )
	{
		ITexture* tex = mNode->getSceneManager()->getVideoDriver()->findTexture(texName);
		setMaterialTexture(textureLayer, tex);
	}

	void RenderComponent::setMaterialTexture( irr::u32 textureLayer, Texture* texture )
	{
		mNode->setMaterialTexture(textureLayer, texture);
	}

	bool RenderComponent::createTriangleSelector( SelectorInfo info )
	{
		ITriangleSelector* sel = nullptr;
		if (info.type == BoundingBox)
		{
			ISceneNode* node = getNode();
			ISceneManager* smgr = node->getSceneManager();

			if (sel = smgr->createTriangleSelectorFromBoundingBox(node))
				node->setTriangleSelector(sel);
		}

		return sel != nullptr;
	}

	void RenderComponent::removeTriangleSelector()
	{
		getNode()->setTriangleSelector(nullptr);
	}

	void RenderComponent::setVisible( bool visible )
	{
		BitOperation::set(mVisibility, 1, visible);
	}

	bool RenderComponent::isVisible() const
	{
		return BitOperation::test(mVisibility, 1);
	}

	bool RenderComponent::isTrulyVisible() const
	{
		return BitOperation::test(mVisibility, 2);
	}

	void RenderComponent::setDebugState( int state )
	{
		getNode()->setDebugDataVisible(state);
	}

	int RenderComponent::getDebugState()
	{
		return getNode()->isDebugDataVisible();
	}

	void RenderComponent::syncWithObject()
	{
		static string BASE_NAME = "Render";

		GameObject* obj = getHostObject();

		bool trulyVisible, localVisible;
		trulyVisible = localVisible = isVisible();

		// visibility is determined by parent's render component
		GameObject* parent;
		if ((parent = obj->getParent()) && localVisible)
		{
			const string& mName = getComponentName();
			Component* comp = parent->findComponent(BASE_NAME);

			RenderComponent* rc;
			trulyVisible = !(rc = polymorphic_downcast<RenderComponent*> (comp)) ||
				(rc->isTrulyVisible());
		}

		BitOperation::set(mVisibility, 2, trulyVisible);
		mNode->setVisible(trulyVisible);

		// sync transform
		// TODO: try to set AbsoluteMatrix directly
		const Matrix& worldTransform = obj->getWorldTransformMatrix();
		mNode->setPosition(worldTransform.getTranslation());
		mNode->setRotation(worldTransform.getRotationDegrees());
		mNode->setScale(worldTransform.getScale());
	}

	void RenderComponent::onStart()
	{
		mNode->setParent(mNode->getSceneManager()->getRootSceneNode());
	}

	void RenderComponent::onUpdate( const ngn::Timeline& )
	{
		
	}

	void RenderComponent::onStop()
	{
		mNode->setParent(nullptr);
	}

	enum 
	{
		MAGIC_NUMBER = 0xcafebabe,
		MAGIC_LENGTH = sizeof(int),
		DATA_LENGTH  = sizeof(RenderComponent*),
		NAME_LENGTH  = DATA_LENGTH + MAGIC_LENGTH,
	};
	RenderComponent* RenderComponent::getComponentFromNode( const irr::scene::ISceneNode* node )
	{
		const char* namedPtr = node->getName();
		int magicNumber;
		void* ptr;
		memcpy(&magicNumber, namedPtr, MAGIC_LENGTH);
		if (magicNumber != MAGIC_NUMBER)
			return nullptr;
		else
		{
			memcpy(&ptr, namedPtr+MAGIC_LENGTH, DATA_LENGTH);
			return static_cast<RenderComponent*> (ptr);
		}
	}

	bool RenderComponent::isCulled() const
	{
		return getNode()->getSceneManager()->isCulled(getNode());
	}

	irr::scene::ITriangleSelector* RenderComponent::getTriangleSelector() const
	{
		return getNode()->getTriangleSelector();
	}

	static void setUserdata2Node( irr::scene::ISceneNode* node, void* userdata )
	{
		if (userdata == nullptr)
		{
			node->setName(stringc());
		}
		else
		{
			char renderptr2name[NAME_LENGTH+1];
			int magicNumber = MAGIC_NUMBER;
			memcpy(renderptr2name, &magicNumber, MAGIC_LENGTH);
			memcpy(renderptr2name+MAGIC_LENGTH, &userdata, DATA_LENGTH);
			renderptr2name[NAME_LENGTH] = '\0';

			stringc codedName(renderptr2name, NAME_LENGTH);
			node->setName(codedName);
		}
	}

}}
