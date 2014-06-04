#include "SpeakerSupport.h"
#include "CPlainTextContent.h"
#include "CDialogueContext.h"
#include "CTickByLetter.h"
#include "CTickAll.h"

#include <Dialogue\ITextContent.hpp>
#include <Dialogue\CDialogueAlignmenter.hpp>
#include <Dialogue\CAlignedDialogueBuilder.hpp>
#include <Dialogue\ITickMethod.hpp>

#include <CEGUI\Window.h>
#include <CEGUI\Font.h>
#include <CEGUI\PropertyHelper.h>
#include <CEGUI\CoordConverter.h>
#include <CEGUI\AnimationManager.h>
#include <CEGUI\AnimationInstance.h>

#include <assert.h>

namespace xihad { namespace dialogue
{
	const CEGUI::String SpeakerSupport::ICON_WINDOW_NAME = "IconWindow";
	const CEGUI::String SpeakerSupport::TEXT_WINDOW_NAME = "TextWindow";
	
	namespace 
	{
		const CEGUI::String AUTO_CONTAINER = "__auto_container__";;
		const CEGUI::String AUTO_NAME_CARD = "__auto_name_card__";
		const CEGUI::String AUTO_NEXT = "__auto_next__";

		const CEGUI::String DARKER_ANIM_EVENT = "Darker";
		const CEGUI::String LIGHTER_ANIM_EVENT= "Lighter";
		const CEGUI::String SHRINK_ANIM_EVENT = "Shrink";
		const CEGUI::String MAGNIFY_ANIM_EVENT= "Magnify";

		const CEGUI::String DIALOGUE_OPEN = "DialogueOpen";
		const CEGUI::String DIALOGUE_CLOSE= "DialogueClose";
	}

	using namespace CEGUI;
	SpeakerSupport::SpeakerSupport(CEGUI::Window& wnd, const CEGUI::String& name,
		ngn::dimension2di paragraphPadding, int lineSpace) : 
			baseWnd(wnd), padding(paragraphPadding), 
			lineSpacing(lineSpace), status(CLOSE)
	{
		getTextWindow()->setProperty("CharacterName", name);
		initWidget();
	}

	void SpeakerSupport::setIcon( const String& icon )
	{
		getIconWindow()->setProperty("Image", icon);
	}

	void SpeakerSupport::open()
	{
		if (status > CLOSE) 
		{
			std::cout << "Dialogue has already opened." << std::endl;
			return;
		}

		baseWnd.fireEvent(DIALOGUE_OPEN, WindowEventArgs(&baseWnd));
		status = OPEN;
	}

	void SpeakerSupport::close()
	{
		if (status < OPEN) 
		{
			std::cout << "Dialogue has already closed." << std::endl;
			return;
		}

		deactive();
		baseWnd.fireEvent(DIALOGUE_CLOSE, WindowEventArgs(&baseWnd));
		status = CLOSE;
	}

	void SpeakerSupport::active()
	{
		if (!checkOpened() || status == ACTIVE) return;

		Window* textWnd = getTextWindow();
		Window* iconWnd = getIconWindow();
		textWnd->fireEvent(MAGNIFY_ANIM_EVENT, WindowEventArgs(textWnd));
		iconWnd->fireEvent(LIGHTER_ANIM_EVENT, WindowEventArgs(iconWnd));
		status = ACTIVE;
	}

	void SpeakerSupport::deactive()
	{
		if (!checkOpened() || status == DEACTIVE) return;

		Window* textWnd = getTextWindow();
		Window* iconWnd = getIconWindow();
		textWnd->fireEvent(SHRINK_ANIM_EVENT, WindowEventArgs(textWnd));
		iconWnd->fireEvent(DARKER_ANIM_EVENT, WindowEventArgs(iconWnd));
		status = DEACTIVE;
	}

	bool SpeakerSupport::checkOpened()
	{
		if (status < OPEN)
		{
			std::cout << "You should open it first" << std::endl;
			return false;
		}

		return true;
	}

	void SpeakerSupport::setIconRelativeX( float xPercent )
	{
		assert(xPercent >= -1 && xPercent <= 1);

		Window* textWnd = getTextWindow();
		Window* iconWnd = getIconWindow();
		Window* nameCard = textWnd->getChild(AUTO_NAME_CARD);

		UDim xPos;
		if (xPercent >= 0)
		{
			setNameCardProperty(false, xPercent, 0);
			xPos = UDim(0, xPercent * textWnd->getParentPixelSize().d_width);
		}
		else
		{
			setNameCardProperty(true, 0, -xPercent);
			xPos = UDim(1, xPercent * textWnd->getParentPixelSize().d_width);
			xPos -= UDim(0, iconWnd->getPixelSize().d_width);
		}

		getIconWindow()->setXPosition(xPos);
	}


	void SpeakerSupport::setNameCardProperty( 
		bool rightAligned, float leftEdge, float rightEdge )
	{
		Window* textWnd = getTextWindow();
		textWnd->setProperty("NameCardRightAligned", 
			PropertyHelper<bool>::toString(rightAligned));
		textWnd->setProperty("LRelativeX", 
			PropertyHelper<float>::toString(leftEdge));
		textWnd->setProperty("RRelativeX",
			PropertyHelper<float>::toString(rightEdge));

	}

	void SpeakerSupport::setDialoguePosition( float x, float y )
	{
		Sizef sz = baseWnd.getPixelSize();

		UDim xPos, yPos;
		if (x >= 0)
			xPos = UDim(0, x);
		else
			xPos = UDim(1, x) - UDim(0, sz.d_width);

		if (y >= 0)
			yPos = UDim(0, y);
		else
			yPos = UDim(1, y) - UDim(0, sz.d_height);

		baseWnd.setPosition(UVector2(xPos, yPos));
	}

	void SpeakerSupport::speak( const String& content )
	{
		dialogue.reset(generateDialog(content));
		getTextWindow()->getChild(AUTO_NEXT)->setVisible(false);
	}

	void SpeakerSupport::speakAll()
	{
		if (checkActive()) 
		{
			ITickMethod* all = new CTickAll(*dialogue);
			dialogue->setTickMethod(all);
			all->drop();
		}
	}

	void SpeakerSupport::setSpeed( float level )
	{
		if (checkActive()) 
		{
			dialogue->getTickMethod()->setTickSpeed(level);
		}
	}

	Window* SpeakerSupport::getTextWindow()
	{
		return baseWnd.getChildRecursive(TEXT_WINDOW_NAME);
	}

	Window* SpeakerSupport::getIconWindow()
	{
		return baseWnd.getChildRecursive(ICON_WINDOW_NAME);
	}

	IDialogue* SpeakerSupport::generateDialog(const String& text)
	{
		Window* textWnd = getTextWindow();

		ITextContent* content = new CPlainTextContent(*textWnd->getFont(), text);
		CDialogueContext* factory = new CDialogueContext(
			*(textWnd->getChild(AUTO_CONTAINER)));

		float margin = PropertyHelper<float>::fromString(
			textWnd->getProperty("TextMargin"));
		CDialogueAlignmenter* alignmenter = new CDialogueAlignmenter(
			(int) (textWnd->getParentPixelSize().d_width*0.9 - 2*margin));

		alignmenter->setKerningHeight(lineSpacing);
		alignmenter->setKerningNewLine(padding);
		CAlignedDialogueBuilder dialogBuilder(factory, alignmenter);
		dialogBuilder.addText(content);

		content->drop();
		factory->drop();
		alignmenter->drop();

		IDialogue* newDialog = dialogBuilder.build();
		ITickMethod* tickMethod = new CTickByLetter(*newDialog, 0.2f);
		newDialog->setTickMethod(tickMethod);
		tickMethod->drop();

		return newDialog;
	}

	void SpeakerSupport::updateSubtitle(float deltaTime)
	{
		if (!dialogue) 
			return;

		if (checkActive())
		{
			if (!isTicking())
				getTextWindow()->getChild(AUTO_NEXT)->setVisible(true);
			else
				dialogue->onUpdate(deltaTime);
		}
	}

	void SpeakerSupport::initWidget()
	{
		Window* iconWnd = getIconWindow();
		iconWnd->setProperty("EnableColor", "FF4F4F4F");
		
		Window* textWnd = getTextWindow();
		textWnd->setSize(USize(UDim(0, 0), UDim(0, 0)));
		textWnd->getChild(AUTO_NEXT)->setVisible(false);

		instanceAnimation(DIALOGUE_OPEN, &baseWnd);
		instanceAnimation(DIALOGUE_CLOSE, &baseWnd);

		instanceAnimation(LIGHTER_ANIM_EVENT, iconWnd);
		instanceAnimation(DARKER_ANIM_EVENT, iconWnd);

		instanceAnimation(MAGNIFY_ANIM_EVENT, textWnd);
		instanceAnimation(SHRINK_ANIM_EVENT, textWnd);
	}

	bool SpeakerSupport::checkActive()
	{
		return dialogue && status == ACTIVE;
	}

	bool SpeakerSupport::isTicking() const
	{
		if (!dialogue)
			return false;

		return dialogue->getVisibility() != dialogue->endVisibility();
	}

	bool SpeakerSupport::canStop() const
	{
		for (AnimationInstance* ai : anims)
		{
			if (ai->isRunning())
				return false;
		}

		return true;
	}

	void SpeakerSupport::instanceAnimation( const String& anim, CEGUI::Window* target )
	{
		AnimationManager& animMgr = AnimationManager::getSingleton();
		AnimationInstance* animInstance = animMgr.instantiateAnimation(anim);
		animInstance->setTargetWindow(target);
		anims.push_back(animInstance);
	}

}}

