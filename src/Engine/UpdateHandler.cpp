#include "UpdateHandler.h"
#include "CppBase/xassert.h"
#include "MemoryLeakDetector.h"
#include <iostream>

namespace xihad { namespace ngn
{
	enum E_Pending_Status
	{
		EPS_STOP	= 0x80000000,
		EPS_ALL_BITS= 0xf0000000,
	};

	inline 
	static bool pendingStatus(int statusBits, E_Pending_Status pendingBit)
	{
		return (statusBits & pendingBit) != 0;
	}

	UpdateHandler::UpdateHandler() : mStatus(BORN) { XIHAD_MLD_NEW_OBJECT; }

	bool UpdateHandler::start()
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

	bool UpdateHandler::update( const Timeline& tl )
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

	bool UpdateHandler::stop()
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

	bool UpdateHandler::intermediateStatus() const
	{
		Status mStatus = status();
		switch (mStatus)
		{
		case BORN:
		case UPDATED:
		case DEAD:
			return false;
		default:
			return true;
		}
	}

	UpdateHandler::Status UpdateHandler::status() const 
	{
		return (Status) (mStatus & (~EPS_ALL_BITS));
	}

	bool UpdateHandler::destroy()  
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
		postcondition(status() == DESTROYING);

		delete this;
		return true;
	}

	UpdateHandler::~UpdateHandler()
	{
		XIHAD_MLD_DEL_OBJECT;

		xassert(status() == DESTROYING && 
			"Never delete UpdateHandler directly, invoke UpdateHandler::stop() instead");
	}

	void UpdateHandler::execPendingCommand(Status preStatus)
	{
		int pendingBits = mStatus & EPS_ALL_BITS;

		mStatus = preStatus;
		if (pendingStatus(pendingBits, EPS_STOP))
			this->stop();
	}

	void UpdateHandler::onDestroy()
	{
	}

}}

