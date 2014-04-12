#include "DialogueSelector.h"

#include <CEGUI\GUIContext.h>
#include <CEGUI\System.h>
#include <CEGUI\Window.h>
#include <iostream>

namespace xihad { namespace dialogue
{
	using namespace CEGUI;

	const CEGUI::String DialogueSelector::LEFT_DIALOG = "LeftDialog";
	const CEGUI::String DialogueSelector::RIGHT_DIALOG = "RightDialog";

	const CEGUI::String DialogueSelector::TEXT_AREA = "TextArea";
	const CEGUI::String DialogueSelector::IMAGE_AREA = "ImageArea";
	const CEGUI::String DialogueSelector::SUBTITLE_CONTAINER = "__auto_container__";
	const CEGUI::String DialogueSelector::NEXT_BUTTON = "__auto_next__";

	const CEGUI::String DialogueSelector::CHARACTER_NAME_PROPERTY = "CharacterName";
	const CEGUI::String DialogueSelector::PORTRAIT_PROPERTY= "Image";

	const CEGUI::String DialogueSelector::DARKER_ANIM_EVENT = "Darker";
	const CEGUI::String DialogueSelector::LIGHTER_ANIM_EVENT = "Lighter";
	const CEGUI::String DialogueSelector::SHRINK_ANIM_EVENT = "Shrink";
	const CEGUI::String DialogueSelector::MAGNIFY_ANIM_EVENT = "Magnify";

	const CEGUI::String DialogueSelector::HIDE_EVENT = "Close";

	DialogueSelector::DialogueSelector(bool startFromLeft /*=true*/)
	{
		GUIContext& context = System::getSingleton().getDefaultGUIContext();
		Window* root = context.getRootWindow();
		left = root->getChild(LEFT_DIALOG);
		right = root->getChild(RIGHT_DIALOG);
		left->setVisible(false);
		right->setVisible(false);
		
		current = startFromLeft ? right : left;
	}

	Window* DialogueSelector::nextDialog( const CEGUI::String& name, const CEGUI::String& image )
	{
		Window* textArea = current->getChild(TEXT_AREA);
		Window* imageArea = current->getChild(IMAGE_AREA);
		if (textArea->getProperty(CHARACTER_NAME_PROPERTY) != name)
		{
			switchDialog();
			textArea->fireEvent(SHRINK_ANIM_EVENT, WindowEventArgs(textArea));
			imageArea->fireEvent(DARKER_ANIM_EVENT, WindowEventArgs(imageArea));

			textArea = current->getChild(TEXT_AREA);
			textArea->setProperty(CHARACTER_NAME_PROPERTY, name);
			textArea->fireEvent(MAGNIFY_ANIM_EVENT, WindowEventArgs(textArea));

			imageArea = current->getChild(IMAGE_AREA);
			imageArea->setProperty(PORTRAIT_PROPERTY, image);
			imageArea->fireEvent(LIGHTER_ANIM_EVENT, WindowEventArgs(imageArea));
		}

		Window* nextButton = textArea->getChild(DialogueSelector::NEXT_BUTTON);
		nextButton->setVisible(false);
		current->setVisible(true);
		return current;
	}

	void DialogueSelector::switchDialog()
	{
		current = current == left ? right : left;
	}

	void DialogueSelector::close()
	{
		Window* textArea = current->getChild(TEXT_AREA);
		textArea->fireEvent(SHRINK_ANIM_EVENT, WindowEventArgs(textArea));
		Window* imageArea = current->getChild(IMAGE_AREA);
		imageArea->fireEvent(DARKER_ANIM_EVENT, WindowEventArgs(imageArea));

		left->fireEvent(HIDE_EVENT, WindowEventArgs(left));
		right->fireEvent(HIDE_EVENT, WindowEventArgs(right));
	}

	Window* DialogueSelector::currentDialog() const
	{
		return current;
	}

	CEGUI::Window* DialogueSelector::getTextContainer( CEGUI::Window* dialogPane )
	{
		return dialogPane->getChildRecursive(SUBTITLE_CONTAINER);
	}

	CEGUI::Window* DialogueSelector::getNextButton( CEGUI::Window* dialogPane )
	{
		return dialogPane->getChildRecursive(NEXT_BUTTON);
	}

}}

