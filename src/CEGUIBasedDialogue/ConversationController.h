#pragma once
#include "Engine\UserEventReceiver.h"
#include "Engine\irr_ptr.h"
#include "Engine\UserEvent.h"
#include "Engine\UpdateHandler.h"

namespace xihad { namespace ngn 
{
	class GameScene;
	class Timeline;
}}

namespace xihad { namespace dialogue
{
	class Conversation;
	class ConversationController : public ngn::UserEventReceiver, public ngn::UpdateHandler 
	{
	public:
		ConversationController(ngn::GameScene& scene, irr_ptr<Conversation> conversation);

		~ConversationController() {}

		/**
		 * 1. Register itself to scene as a event receiver, then it can receive user input event.
		 * 2. Register itself to scene as a update handler, then it can be updated.
		 * 
		 * You'd better not call this function more than once, althogh it wouldn't be register again.
		 * 
		 * @param autoUnregister 
		 *		if true, this event receiver will unregister from scene when conversation end.
		 *		otherwise, remember to unregister it from scene by manually.
		 */
		void registerToScene(bool autoUnregister = true);

		virtual int onKeyEvent( const ngn::KeyEvent& event, int argFromPreviousReceiver );
		
		virtual int onMouseEvent( const ngn::MouseEvent& event, int argFromPreviousReceiver );
	
	protected:	
		virtual void onStart();

		virtual void onUpdate( const ngn::Timeline& timeline);

		virtual void onStop();

	private:
		ngn::GameScene& scene;
		irr_ptr<Conversation> conversation;
		bool autoUnregister;
	};
}}