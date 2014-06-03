#include "Conversation.h"
#include "SpeakerSupport.h"

#include <CEGUI\System.h>
#include <CEGUI\Window.h>
#include <CEGUI\GUIContext.h>

#include <algorithm>

namespace xihad { namespace dialogue
{
	using namespace CEGUI;

	Conversation::Conversation()
	{

	}

	Conversation::~Conversation()
	{

	}

	void Conversation::onDestroy()
	{
		std::for_each(speakers.begin(), speakers.end(), 
		[](SpeakerSupport* p)
		{
			Window* wnd = p->getWindow();
			delete p; 

			Window* root = System::getSingleton().
				getDefaultGUIContext().getRootWindow();
			root->destroyChild(wnd);
		});
	}

	SpeakerSupport* Conversation::newSpeaker( const String& name, 
		ngn::dimension2di paragraphPadding /*= ngn::dimension2di()*/, 
		int lineSpace /*= 0*/, int width /*= 500*/, int height /*= 500*/ )
	{
		String wndName = name + speakers.size();
		Window* root = System::getSingleton().getDefaultGUIContext().getRootWindow();
		
		Window* baseWnd = root->createChild("DefaultWindow", wndName);
		baseWnd->setSize(USize(UDim(0, (float) width), UDim(0, (float) height)));
		baseWnd->setVisible(false);

		Window* iconWnd = baseWnd->createChild(
			"Xihad/Image", SpeakerSupport::ICON_WINDOW_NAME);
		iconWnd->setSize(USize(UDim(0.8f, 0), UDim(0.95f, 0)));
		iconWnd->setProperty("VertImageFormat", "BottomAligned");

		Window* textPanel = baseWnd->createChild("DefaultWindow", "TextPanel");
		textPanel->setSize(USize(UDim(0.9f, 0), UDim(0.35f, 0)));
		textPanel->setPosition(UVector2(UDim(0.05f, 0), UDim(0.6f, 0)));

		Window* textWnd = textPanel->createChild(
			"Xihad/Dialog", SpeakerSupport::TEXT_WINDOW_NAME);

		SpeakerSupport* speaker = new SpeakerSupport(
			*baseWnd, name, paragraphPadding, lineSpace);
		
		speakers.push_back(speaker);
		return speaker;
	}

	void Conversation::onStart()
	{
	}

	void Conversation::onUpdate( const ngn::Timeline& tl)
	{
		std::for_each(speakers.begin(), speakers.end(), 
		[&](SpeakerSupport* p)
		{
			p->updateSubtitle(tl.getLastTimeChange());
		});
	}

	void Conversation::onStop()
	{

	}

}}