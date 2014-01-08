#pragma once
#include "MessageListener.h"
#include "Engine/GameScene.h"

namespace xihad { namespace ngn
{
	class BaseMessageListener : public MessageListener
	{
	public:
		BaseMessageListener();

		virtual Destroyer* getDestroyer() const;

		virtual bool destroy();

	protected:
		virtual void onDestroy();

		virtual void setDestroyer( Destroyer* lifeManager );

		~BaseMessageListener();

	private:
		Destroyer* mDestroyer;
	};
}}

