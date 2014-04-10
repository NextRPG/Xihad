#pragma once
#include <CEGUI\String.h>
#include <Engine\dimension2d.h>
#include <Engine\irr_ptr.h>
#include <irrlicht\IReferenceCounted.h>

#include <map>
#include <queue>

#include "DialogueSelector.h"

namespace CEGUI
{
	class Window;
}

namespace xihad { namespace dialogue
{
	using CEGUI::String;
	class IDialogue;
	class IDialogueBuilder;
	class Conversation : public irr::IReferenceCounted
	{
	public:
		Conversation(
			int widthLimit,
			int lineSpacing = 0, 
			const ngn::dimension2di& paragrahPadding = ngn::dimension2di(),
			bool startFromLeft = true);
		
		virtual ~Conversation() {}

		 //	Speaker must be set before speak.
		void setSpeaker(const String& name, const String& defaultEmotion);

		void speak(const String& name, const String& content, const String& emotion);

		void speak(const String& name, const String& content);

		void start();

		void update(float lastElapsed);
		
		void speedUp();

		void slowDown();

		void skipSubtitleAnimation();

		void stop();
		/**
		 * start next words.
		 * 
		 * @return true if succeed, false if current sutitle is playing 
		 */
		bool nextSpeakPiece();

		/**
		 * @return true if all words is finished.
		 */
		bool isFinished();

	private:
		struct SpeakPiece : public irr::IReferenceCounted
		{
			SpeakPiece(const String& pname, const String& pcontent, const String& pimage)
				: name(pname), content(pcontent), image(pimage)
			{
			}

			const String name;
			const String content;
			const String image;
		};

		IDialogue* Conversation::generateDialogue(CEGUI::Window& dialog, const String& text);

		void speak_impl(const String& name, const String& content, const String& image);
		
		void speakPiece(const SpeakPiece& piece);

		/**
		 * @return return false if currentSubtitle is null or reach end
		 */
		bool isSubtitlePlaying();

		String getFullImageName(const String& name, const String& emotion);

	private:
		std::map<String, String> mSpeakers;
		DialogueSelector selector;

		int mWidthLimit;
		int mLineSpacing;
		ngn::dimension2di mParagrahPadding;
		
		IDialogue* currentSubtitle;

		std::queue<irr_ptr<SpeakPiece>> mPieces;
	};
}}
