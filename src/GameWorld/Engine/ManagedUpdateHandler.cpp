#include "ManagedUpdateHandler.h"
#include "Destroyer.h"
#include "CppBase/xassert.h"
#include <iostream>

namespace xihad { namespace ngn
{
	enum E_Pending_Status
	{
		EPS_STOP	= 0x80000000,
		EPS_ALL_BITS= 0xf0000000,
	};

	inline static bool pendingStatus(int statusBits, E_Pending_Status pendingBit)
	{
		return (statusBits & pendingBit) != 0;
	}

	ManagedUpdateHandler::ManagedUpdateHandler() : 
		mStatus(BORN), mLifeManager(nullptr)
	{
	}

	bool ManagedUpdateHandler::start()
	{
		if (status() != BORN) return false;

		precondition(!intermediateStatus());
		precondition(!pendingStatus(mStatus, EPS_ALL_BITS));
		mStatus = STARTING;
		onStart();
		postcondition(status() == STARTING);
		execPendingCommand(UPDATED);

		return true;
	}

	bool ManagedUpdateHandler::update( const Timeline& tl )
	{
		if (status() != UPDATED) return false;

		precondition(!intermediateStatus());
		precondition(!pendingStatus(mStatus, EPS_ALL_BITS));
		mStatus = UPDATING;
		onUpdate(tl);
		postcondition(status() == UPDATING);
		execPendingCommand(UPDATED);

		return true;
	}

	bool ManagedUpdateHandler::stop()
	{
		if (status() == STARTING || status() == UPDATING)
		{
			mStatus |= EPS_STOP;	// remind self to stop after starting or updating
			return false;
		}

		if (status() == UPDATED)
		{
			mStatus = STOPPING;
			onStop();
			postcondition(status() == STOPPING);
			mStatus = DEAD;

			return true;
		}

		return false;
	}

	UpdateHandler::Status ManagedUpdateHandler::status() const 
	{
		return (Status) (mStatus & (~EPS_ALL_BITS));
	}

	bool ManagedUpdateHandler::destroy()  
	{
		if (intermediateStatus())
		{
			std::cerr << "Attempt to destroy UpdateHandler which is in intermediate state" << std::endl;
			return false;
		}

		// Attempt to stop self
		this->stop();

		precondition(status() == BORN || status() == DEAD);
		mStatus = DESTROYING;
		onDestroy();
		if (mLifeManager)
			mLifeManager->onChildDestroy(this);

		postcondition(status() == DESTROYING);

		delete this;
		return true;
	}

	void ManagedUpdateHandler::setDestroyer( Destroyer* m )  
	{
		mLifeManager = m;
	}

	Destroyer* ManagedUpdateHandler::getDestroyer() const  
	{
		return mLifeManager;
	}

	ManagedUpdateHandler::~ManagedUpdateHandler()
	{
		xassert(status() == DESTROYING && 
			"Never delete UpdateHandler directly, invoke UpdateHandler::destroy() instead");
	}

	void ManagedUpdateHandler::execPendingCommand(Status preStatus)
	{
		int pendingBits = mStatus & EPS_ALL_BITS;

		mStatus = preStatus;
		if (pendingStatus(pendingBits, EPS_STOP))
			this->stop();
	}

	void ManagedUpdateHandler::onDestroy()
	{
	}

	bool ManagedUpdateHandler::isDestroying() const
	{
		return mStatus == DESTROYING;
	}

}}

