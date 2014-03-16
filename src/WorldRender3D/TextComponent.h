#pragma once
#include "BillboardComponent.h"


namespace irr { namespace scene
{
	class IBillboardTextSceneNode;
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

	private:
		IBillboardTextSceneNode* getNode() const;
	};
}}

