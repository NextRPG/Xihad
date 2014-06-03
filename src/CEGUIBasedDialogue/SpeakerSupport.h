#pragma once
#include <xmemory>
#include <Engine\dimension2d.h>
#include <Dialogue\IDialogue.hpp>

namespace CEGUI
{
	class Window;
	class String;
}

namespace xihad { namespace dialogue
{
	using CEGUI::String;
	class SpeakerSupport
	{
	public:
		SpeakerSupport(CEGUI::Window& wnd, 
			const CEGUI::String& name, 
			ngn::dimension2di paragraphPadding, 
			int lineSpace);

		void open();

		void active();

		void deactive();

		void close();

		void setIcon(const String& icon);

		void setIConRelativeX(float xPercent);

		void setDialoguePosition(float x, float y);

		void speak(const String& content);

		void speakAll();

		void setSpeed(float level);

		CEGUI::Window* getWindow()
		{
			return &baseWnd;
		}

		void updateSubtitle(float deltaTime);

	public:
		static const CEGUI::String TEXT_WINDOW_NAME;
		static const CEGUI::String ICON_WINDOW_NAME;

	private:
		IDialogue* generateDialog(const String& text);

		CEGUI::Window* getTextWindow();

		CEGUI::Window* getIconWindow();

		void setNameCardProperty(bool rightAligned, float leftEdge, float rightEdge);
	
		bool checkOpened();

		bool checkActive();

		void initWidget();

	private:
		enum Status 
		{
			CLOSE = 0,
			OPEN = 1,
			DEACTIVE = 2,
			ACTIVE = 3,
		};

		CEGUI::Window& baseWnd;
		Status status;
		std::auto_ptr<IDialogue> dialogue;

		ngn::dimension2di padding;
		int lineSpacing;
	
	};
}}