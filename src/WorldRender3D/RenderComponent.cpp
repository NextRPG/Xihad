#include "RenderComponent.h"
#include <irrlicht/ISceneNode.h>
#include <irrlicht/ISceneManager.h>
#include <irrlicht/IVideoDriver.h>
#include <Engine/GameObject.h>
#include <Engine/Transform.h>
#include <CppBase/BitOperation.h>
#include <boost/cast.hpp>
#include "TextureManager.h"

using namespace boost;
namespace xihad { namespace render3d
{
	using namespace core;
	using namespace video;
	using namespace ngn;

	using ::std::string;

	// fwd
	static void setUserdata2Node(ISceneNode* node, RenderComponent* ud);

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
		setUserdata2Node(mNode.get(), 0);
		xassert(RenderComponent::getComponentFromNode(mNode.get()) == nullptr);
	}


	RenderComponent* RenderComponent::createSkyDomeComponent(
		const std::string& name, ngn::GameObject& host, const ngn::Properties& param, 
		ISceneManager* smgr, TextureManager* texManager )
	{
		ITexture* tex = nullptr;
		if (const char* path = param.getString("texture"))
			tex = texManager->getTexture(path);

		if (!tex) return nullptr;

		int horiRes = param.getInt("horiRes", 16);
		int vertRes = param.getInt("vertRes", 8);
		float texPercent = param.getFloat("texPercent", 0.9f);
		float spherePercent = param.getFloat("spherePercent", 2.f);
		float radius = param.getFloat("radius", 1000.f);

		ISceneNode* node = smgr->addSkyDomeSceneNode(tex, horiRes, vertRes, 
			texPercent, spherePercent, radius);

		return new RenderComponent(name, host, node);
	}


	const RenderComponent::AABB& RenderComponent::getBoundingBox() const
	{
		return mNode->getBoundingBox();
	}

	const RenderComponent::AABB RenderComponent::getTransformedBoundingBox() const
	{
		return mNode->getTransformedBoundingBox();
	}

	RenderComponent::Material& RenderComponent::getMaterial( u32 num )
	{
		return mNode->getMaterial(num);
	}

	u32 RenderComponent::getMaterialCount() const
	{
		return mNode->getMaterialCount();
	}

	void RenderComponent::setMaterialFlag( video::E_MATERIAL_FLAG flag, bool newValue )
	{
		mNode->setMaterialFlag(flag, newValue);
	}

	void RenderComponent::setMaterialTexture( u32 textureLayer, const c8* texName )
	{
		ITexture* tex = mNode->getSceneManager()->getVideoDriver()->findTexture(texName);
		setMaterialTexture(textureLayer, tex);
	}

	void RenderComponent::setMaterialTexture( u32 textureLayer, Texture* texture )
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
//		static string BASE_NAME = "Render";
		GameObject* obj = getHostObject();

// 		bool trulyVisible, localVisible;
// 		trulyVisible = localVisible = isVisible();

		// visibility is determined by parent's render component
// 		GameObject* parent;
// 		if ((parent = obj->getParent()) && localVisible)
// 		{
// 			const string& mName = getComponentName();
// 			Component* comp = parent->findComponent(BASE_NAME);
// 
// 			RenderComponent* rc;
// 			trulyVisible = !(rc = polymorphic_downcast<RenderComponent*> (comp)) ||
// 				(rc->isTrulyVisible());
// 		}
// 
// 		BitOperation::set(mVisibility, 2, trulyVisible);
		mNode->setVisible(isVisible());

		// sync transform
		// TODO: try to set AbsoluteMatrix directly
		const Transform& worldTransform = obj->getWorldTransform();
		mNode->setPosition(worldTransform.getTranslation());
		mNode->setRotation(worldTransform.getRotation());
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

	bool RenderComponent::isCulled() const
	{
		return getNode()->getSceneManager()->isCulled(getNode());
	}

	ITriangleSelector* RenderComponent::getTriangleSelector() const
	{
		return getNode()->getTriangleSelector();
	}


	enum 
	{
		MAGIC_NUMBER = 0xcafebabe,
		MAGIC_LENGTH = sizeof(int),
		DATA_LENGTH  = sizeof(RenderComponent*),
		NAME_LENGTH  = DATA_LENGTH + MAGIC_LENGTH,
	};

	union RenderComponentIntrusiveData
	{
		struct Data
		{
			int magicNumber;
			RenderComponent* ptr;
		} rcdata;

		char name[NAME_LENGTH];
	};

	static void setUserdata2Node( ISceneNode* node, RenderComponent* userdata )
	{
		if (userdata == nullptr)
		{
			node->setName(stringc());
		}
		else
		{
			RenderComponentIntrusiveData intrsive;
			intrsive.rcdata.magicNumber = MAGIC_NUMBER;
			intrsive.rcdata.ptr = userdata;
			stringc codedName(intrsive.name, NAME_LENGTH);
			node->setName(codedName);
		}
	}

	RenderComponent* RenderComponent::getComponentFromNode( const ISceneNode* node )
	{
		char* namedPtr = const_cast<char*>(node->getName());
		RenderComponentIntrusiveData* intrusive = 
			reinterpret_cast<RenderComponentIntrusiveData*>(namedPtr);

		if (intrusive->rcdata.magicNumber == MAGIC_NUMBER)
			return intrusive->rcdata.ptr;

		return 0;
	}

}}
