#pragma once
#include <list>
#include <Engine\dimension2d.h>
#include <Engine\UpdateHandler.h>
#include <Engine\Timeline.h>

namespace CEGUI
{
	class String;
}

namespace xihad { namespace dialogue
{
	class SpeakerSupport;
	class Conversation : public ngn::UpdateHandler
	{
	public:
		explicit Conversation();

		virtual ~Conversation();

		SpeakerSupport* newSpeaker(
			const CEGUI::String& name, 
			ngn::dimension2di paragraphPadding = ngn::dimension2di(), 
			int lineSpace = 0,
			int width = 500, int height = 500);

		void requestStop();

	protected:
		virtual void onStart() override;

		virtual void onUpdate( const ngn::Timeline& tl) override;

		virtual void onStop() override;

		virtual void onDestroy() override;

	private:
		typedef std::list<SpeakerSupport*> Container;
		Container speakers;
		
		bool stopPendding;
	};
}}
