#pragma once
#include <boost\scoped_ptr.hpp>
#include <Engine\dimension2d.h>
#include <Dialogue\IDialogue.hpp>
#include <list>

namespace CEGUI
{
	class Window;
	class String;
	class AnimationInstance;
}

namespace xihad { namespace dialogue
{
	using CEGUI::String;
	class SpeakerSupport
	{
	public:
		enum Status 
		{
			CLOSE,
			OPEN,
			DEACTIVE,
			ACTIVE,
		};

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

		void setIconRelativeX(float xPercent);

		void setDialoguePosition(float x, float y);

		void speak(const String& content);

		void speakAll();

		void setSpeed(float level);


		CEGUI::Window* getWindow()
		{
			return &baseWnd;
		}

		void updateSubtitle(float deltaTime);

		Status getStatus() const
		{
			return status;
		}

		bool isTicking() const;

		bool canStop() const;

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

		void instanceAnimation(const String& anim, CEGUI::Window* target);

	private:
		CEGUI::Window& baseWnd;
		boost::scoped_ptr<IDialogue> dialogue;
		Status status;
		std::list<CEGUI::AnimationInstance*> anims;

		ngn::dimension2di padding;
		int lineSpacing;
	
	};
}}