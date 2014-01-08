#pragma once
#include "Engine/Component.h"
#include "Engine/irr_ptr.h"
#include "irrlicht/aabbox3d.h"
#include "irrlicht/matrix4.h"
#include "irrlicht/SMaterial.h"
#include "irrlicht/EDebugSceneTypes.h"

namespace irr { namespace scene 
{
	class ITriangleSelector;
	class ISceneNode;
}}

namespace xihad { namespace render3d
{
	typedef irr::scene::E_DEBUG_SCENE_TYPE DebugState;

	enum SelectorType
	{
		BoundingBox, Octree, Terrian, Stupid
	};

	struct SelectorInfo
	{
		SelectorType type;
		union Parameter
		{
			int minimalPolysPerNode;	// use for Octree
			int LOD;					// use for terrian
			int useless;				// for other type
		} param;

		SelectorInfo(SelectorType t = BoundingBox, int p = 0) : type(t)
		{
			param.minimalPolysPerNode = p;
			// param.LOD = p;
			// param.useless = p;
		}
	};

	/************************************************************************/
	/* You cannot set the node's parent explicitly, because the hierarchy has
	 * been managed by GameObject and this Component.
	/************************************************************************/
	class RenderComponent : public ngn::Component
	{
	public:
		typedef irr::core::aabbox3d<irr::f32> AABB;
		typedef irr::core::matrix4 Matrix;
		typedef irr::video::SMaterial Material;
		typedef irr::video::ITexture Texture;

	public:
		static RenderComponent* getComponentFromNode(
							const irr::scene::ISceneNode* node);

	public:
		DEFINE_VISITABLE

		RenderComponent(const std::string& name, ngn::GameObject& host, 
						irr::scene::ISceneNode* node);

		// BoundingBox-Related
		const AABB& getBoundingBox() const;
		const AABB getTransformedBoundingBox() const;

		// Material-Related
		Material& getMaterial(irr::u32 num);
		irr::u32 getMaterialCount() const;
		void setMaterialFlag(irr::video::E_MATERIAL_FLAG flag, bool newValue);
		void setMaterialTexture(irr::u32 textureLayer, const irr::c8* texName);
		void setMaterialTexture(irr::u32 textureLayer, Texture* texture);

		// Triangle-Selector
		virtual bool createTriangleSelector(SelectorInfo info = SelectorInfo());
		irr::scene::ITriangleSelector* getTriangleSelector() const;
		void removeTriangleSelector();

		// Visibility
		virtual void setVisible(bool visible);
		virtual bool isVisible() const;
		virtual bool isTrulyVisible() const;

		// Debug-Info 
		// @see DebugState
		void setDebugState(int state);
		int getDebugState();

		bool isCulled() const;

	protected:
		virtual ~RenderComponent();

		virtual void onStart();

		virtual void onUpdate( const ngn::Timeline& );

		virtual void onStop();

		irr::scene::ISceneNode* getNode()
		{
			return mNode.get();
		}

		irr::scene::ISceneNode const * getNode() const
		{
			return mNode.get();
		}

	private:
		const irr_ptr<irr::scene::ISceneNode> mNode;
		int mVisibility;
	};
}}

