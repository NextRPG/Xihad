#include "TextComponent.h"
#include <irrlicht/IBillboardTextSceneNode.h>
#include <irrlicht/IGUIEnvironment.h>
#include <irrlicht/ISceneManager.h>
#include <irrlicht/IFileSystem.h>
#include <irrlicht/IGUIFontBuilder.h>
#include <Engine/Properties.h>
#include <CppBase/StringConventer.h>
#include <irrlicht/IGUIFontBitmap.h>
#include "CBinaryFontReader.h"

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

		gui::IGUIFont* font = getFont(smgr, params.getString("font"));
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

	gui::IGUIFont* TextComponent::getFont( scene::ISceneManager* smgr, const char* fontFilename )
	{
		if (!fontFilename) 
			return smgr->getGUIEnvironment()->getBuiltInFont();

		if (gui::IGUIFont* font = smgr->getGUIEnvironment()->getFont(fontFilename))
			return font;

		return loadFont(smgr, fontFilename);
	}

	gui::IGUIFont* TextComponent::loadFont( scene::ISceneManager* smgr, const char* fontFilename )
	{
		core::stringc path = fontFilename;
		io::IFileSystem* fs = smgr->getFileSystem();
		gui::IGUIFont* font = 0;
		io::IReadFile* istream = fs->createAndOpenFile(path);
		if (istream)
		{
			io::path workingDir = fs->getWorkingDirectory();

			///< Change working dir
			fs->changeWorkingDirectoryTo(fs->getFileDir(path));
			gui::IGUIFontBuilder* fb = smgr->getGUIEnvironment()->createFontBuilder(path);

			font::CBinaryFontReader reader;
			if (font = reader.loadFont(istream, fb))
			{
				smgr->getGUIEnvironment()->addFont(path, font);
				font->drop();
			}

			istream->drop();
			fb->drop();

			///> Switch back working dir
			fs->changeWorkingDirectoryTo(workingDir);
		}

		return font;
	}

	void TextComponent::adjustToHeight( float height )
	{
		getNode()->adjustToHeight(height);
	}

}}

