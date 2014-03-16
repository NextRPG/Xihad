#include "TextComponent.h"
#include <irrlicht/IBillboardTextSceneNode.h>
#include <irrlicht/IGUIEnvironment.h>
#include <irrlicht/ISceneManager.h>
#include <Engine/Properties.h>
#include <CppBase/StringConventer.h>

using namespace std;
namespace xihad { namespace render3d
{
	using namespace ngn;

	TextComponent::TextComponent(const string& name, GameObject& host, IBillboardTextSceneNode* node) :
		BillboardComponent(name, host, node)
	{
	}

	TextComponent* TextComponent::create(const string& name, GameObject& host, 
		const Properties& params, ISceneManager* smgr)
	{
		std::wstring wtext;
		if (const char* text = params.getString("text", ""))
			StringConventer::UTF8_to_wstring(text, wtext);

		gui::IGUIFont* font = 0;
		if (const char* fontFilename = params.getString("font"))
			font = smgr->getGUIEnvironment()->getFont(fontFilename);
		else 
			font = smgr->getGUIEnvironment()->getBuiltInFont();

		IBillboardTextSceneNode* tnode = smgr->addBillboardTextSceneNode(font, wtext.c_str());
		return new TextComponent(name, host, tnode);
	}

	void TextComponent::setText( const char* text )
	{
		wstring wtext;
		StringConventer::UTF8_to_wstring(text, wtext);
		getNode()->setText(wtext.c_str());
	}

	IBillboardTextSceneNode* TextComponent::getNode() const
	{
		return static_cast<IBillboardTextSceneNode*>(RenderComponent::getNode());
	}

}}

