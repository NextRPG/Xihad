#pragma once
#include "Engine\Destroyable.h"

namespace xihad { namespace ngn
{
	class MessageParam;
	class GameObject;
	class MessageListener : public Destroyable
	{
	public:
		typedef MessageParam Parameter;

	public:
		virtual void receive(GameObject& pSource, const Parameter& pEvent) = 0;

		virtual Destroyer* getDestroyer() const = 0;

		virtual bool destroy() = 0;

	protected:
		virtual void setDestroyer(Destroyer* lifeManager) = 0;

		virtual ~MessageListener() {} 
	};
}}