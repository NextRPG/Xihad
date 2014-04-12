#pragma once
#include "BillboardComponent.h"


namespace irr { namespace scene
{
	class IBillboardTextSceneNode;
}}

namespace irr { namespace gui
{
	class IGUIFont;
}}

namespace xihad { namespace render3d
{
	class TextComponent : public BillboardComponent
	{
	public:
		DEFINE_VISITABLE;
		
		TextComponent(const std::string& name, ngn::GameObject& host, IBillboardTextSceneNode* node );

		static TextComponent* create(
			const std::string& name, ngn::GameObject& host, const ngn::Properties& params,
			ISceneManager* smgr);

		virtual void setText(const char* text);

		virtual void adjustToHeight(float height);

	private:
		static gui::IGUIFont* getFont(scene::ISceneManager* smgr, const char* fontFilename);
		static gui::IGUIFont* loadFont(scene::ISceneManager* smgr, const char* fontFilename);

	private:
		IBillboardTextSceneNode* getNode() const;
	};
}}

