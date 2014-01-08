#include "BaseMessageListener.h"

namespace xihad { namespace ngn
{
	BaseMessageListener::BaseMessageListener() : mDestroyer(nullptr)
	{
	}

	Destroyer* BaseMessageListener::getDestroyer() const
	{
		return mDestroyer;
	}

	bool BaseMessageListener::destroy()
	{
		onDestroy();

		if (mDestroyer) 
			mDestroyer->onChildDestroy(this);

		delete this;
		return true;
	}

	void BaseMessageListener::onDestroy()
	{
	}

	void BaseMessageListener::setDestroyer( Destroyer* lifeManager )
	{
		mDestroyer = lifeManager;
	}

	BaseMessageListener::~BaseMessageListener()
	{
	}

}}

