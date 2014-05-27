#include "Conversation.h"

#include <Dialogue\IDialogue.hpp>
#include <Dialogue\CDialogueAlignmenter.hpp>
#include <Dialogue\CAlignedDialogueBuilder.hpp>

#include <CEGUI\Font.h>
#include <CEGUI\Window.h>
#include <CEGUI\ImageManager.h>
#include <CEGUI\widgets\PushButton.h>

#include <assert.h>

#include "CPlainTextContent.h"
#include "CTickByLetter.h"
#include "CDialogueContext.h"
#include "CTickAll.h"

namespace xihad { namespace dialogue
{
	using namespace CEGUI;

	Conversation::Conversation( int widthLimit, int lineSpacing /*= 0*/, 
		const ngn::dimension2di& paragrahPadding /*= ngn::dimension2di()*/,
		bool startFromLeft /*= true*/ )
		: mWidthLimit(widthLimit), 
		mLineSpacing(lineSpacing), mParagrahPadding(paragrahPadding), 
		selector(startFromLeft), currentSubtitle(nullptr)
	{
		XIHAD_MLD_NEW_OBJECT;
	}

	Conversation::~Conversation()
	{
		XIHAD_MLD_DEL_OBJECT;
	}

	void Conversation::speak( const String& name, const String& content )
	{
		static const String NULL_IMAGE = "";
		speak_impl(name, content, NULL_IMAGE);
	}

	void Conversation::speak( const String& name, const String& content, const String& image )
	{
		speak_impl(name, content, image);
	}

	void Conversation::speak_impl( const String& name, const String& content, const String& image )
	{
		SpeakPiece* piece = new SpeakPiece(name, content, image);
		mPieces.push(piece);
		piece->drop();

		// TODO ²ð·Öpiece
	}

	void Conversation::speakPiece(const SpeakPiece& piece)
	{
		Window* dialogPane = selector.nextDialog(piece.name, piece.image);
		Window* container = DialogueSelector::getTextContainer(dialogPane);
		currentSubtitle = generateDialogue(*container, piece.content);
	}

	IDialogue* Conversation::generateDialogue(Window& container, const CEGUI::String& text)
	{
		const Font* font  = container.getFont();
		ITextContent* content = new CPlainTextContent(*font, text);
		CDialogueContext* factory = new CDialogueContext(container);
		CDialogueAlignmenter* alignmenter = new CDialogueAlignmenter(mWidthLimit);

		alignmenter->setKerningHeight(mLineSpacing);
		alignmenter->setKerningNewLine(mParagrahPadding);
		CAlignedDialogueBuilder dialogBuilder(factory, alignmenter);
		dialogBuilder.addText(content);

		content->drop();
		factory->drop();
		alignmenter->drop();

		IDialogue* dialog = dialogBuilder.build();
		ITickMethod* tickMethod = new CTickByLetter(*dialog, 0.2f);
		dialog->setTickMethod(tickMethod);
		tickMethod->drop();

		return dialog;
	}

	void Conversation::speedUp()
	{
		if (currentSubtitle)
			currentSubtitle->getTickMethod()->setTickSpeed(6);
	}

	void Conversation::slowDown()
	{
		if (currentSubtitle)
			currentSubtitle->getTickMethod()->setTickSpeed(1);
	}

	void Conversation::skipSubtitleAnimation()
	{
		if (!isSubtitlePlaying())
			return;

		ITickMethod* all = new CTickAll(*currentSubtitle);
		currentSubtitle->setTickMethod(all);
		all->drop();
	}

	void Conversation::update(float lastElapsed)
	{
		if (!currentSubtitle) return;

		if (!isSubtitlePlaying())
		{
			Window* dialogPane = selector.currentDialog();
			Window* nextButton = DialogueSelector::getNextButton(dialogPane);
			nextButton->setVisible(true);
			return;							// reach end, and no need to update dialogue. 
		}

		currentSubtitle->onUpdate(lastElapsed);
	}

	bool Conversation::nextSpeakPiece()
	{
		if (isSubtitlePlaying())
			return false;

		delete currentSubtitle;				// delete nullptr is safe.
		currentSubtitle = nullptr;
		if (!mPieces.empty()) 
		{
			auto& piece = mPieces.front();
			speakPiece(*piece.get());
			mPieces.pop();
			return true;
		}

		return false;
	}

	bool Conversation::isSubtitlePlaying()
	{
		return currentSubtitle ? 
			!(currentSubtitle->getVisibility() == currentSubtitle->endVisibility()):
		false;
	}

	bool Conversation::isFinished()
	{
		return mPieces.empty() && !currentSubtitle;
	}

	void Conversation::start()
	{
		nextSpeakPiece();
	}

	void Conversation::stop()
	{
		selector.close();
	}

}}