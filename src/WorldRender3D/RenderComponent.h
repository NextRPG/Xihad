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
	class ISceneManager;
}}

namespace xihad { namespace ngn
{
	class Properties;
}}

namespace xihad { namespace render3d
{
	using namespace irr;
	using namespace scene;
	typedef E_DEBUG_SCENE_TYPE DebugState;

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


	class TextureManager;
	/************************************************************************/
	/* You cannot set the node's parent explicitly, because the hierarchy has
	 * been managed by GameObject and this Component.
	/************************************************************************/
	class RenderComponent : public ngn::Component
	{
	public:
		typedef core::aabbox3d<f32> AABB;
		typedef core::matrix4 Matrix;
		typedef video::SMaterial Material;
		typedef video::ITexture Texture;

	public:
		static RenderComponent* getComponentFromNode(const ISceneNode* node);

	public:
		DEFINE_VISITABLE

		RenderComponent(const std::string& name, ngn::GameObject& host, ISceneNode* node);

		static RenderComponent* createSkyDomeComponent(
			const std::string& name, ngn::GameObject& host, const ngn::Properties& params,
			ISceneManager* smgr, TextureManager*);

		// BoundingBox-Related
		const AABB& getBoundingBox() const;
		const AABB getTransformedAABB() const;

		// Material-Related
		Material& getMaterial(u32 num);
		u32 getMaterialCount() const;
		void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newValue);
		void setMaterialTexture(u32 textureLayer, Texture* texture);

		// Triangle-Selector
		virtual bool createTriangleSelector(SelectorInfo info = SelectorInfo());
		ITriangleSelector* getTriangleSelector() const;
		void removeSelector();

		// Visibility
		virtual void setVisible(bool visible);
		virtual bool isVisible() const;
		virtual bool isTrulyVisible() const;

		// Debug-Info 
		// @see DebugState
		void setDebugState(int state);
		int getDebugState();

		bool isCulled() const;

		ISceneNode* getNode()
		{
			return mNode.get();
		}

		virtual void syncWithObject();

	protected:
		virtual ~RenderComponent();

		virtual void onStart();

		virtual void onUpdate( const ngn::Timeline& );

		virtual void onStop();

		ISceneNode * getNode() const { return mNode.get(); }

	private:
		const irr_ptr<ISceneNode> mNode;
		int mVisibility;
	};
}}

