#pragma once
#include "CppBase\xassert.h"
#include "Engine\MemoryLeakDetector.h"

namespace xihad
{
	class ReferenceCounted
	{
	public:
		//! Constructor.
		ReferenceCounted() 
			: ReferenceCounter(1) 
#ifdef _DEBUG
			, DebugName(0)
#endif
		{
			XIHAD_MLD_NEW_OBJECT;
		}

		//! Destructor.
		virtual ~ReferenceCounted()
		{
			XIHAD_MLD_DEL_OBJECT;
		}

		void grab() const { ++ReferenceCounter; }

		bool drop() const
		{
			// someone is doing bad reference counting.
			xassert(ReferenceCounter > 0)

			--ReferenceCounter;
			if (!ReferenceCounter)
			{
				delete this;
				return true;
			}

			return false;
		}

		//! Get the reference count.
		/** \return Current value of the reference counter. */
		int getReferenceCount() const
		{
			return ReferenceCounter;
		}

	private:
		//! The reference counter. Mutable to do reference counting on const objects.
		mutable int ReferenceCounter;

#ifdef _DEBUG
	public:
		//! Returns the debug name of the object.
		/** The Debugname may only be set and changed by the object
		itself. This method should only be used in Debug mode.
		\return Returns a string, previously set by setDebugName(); */
		const char* getDebugName() const
		{
			return DebugName;
		}

	protected:

		//! Sets the debug name of the object.
		/** The Debugname may only be set and changed by the object
		itself. This method should only be used in Debug mode.
		\param newName: New debug name to set. */
		void setDebugName(const char* newName)
		{
			DebugName = newName;
		}

	private:

		//! The debug name.
		const char* DebugName;
#else
	public:
		const char* getDebugName() const
		{
			return "RELEASE_VERSION_DONT_HAVE_DEBUG_NAME";
		}

	protected:
		void setDebugName(const char* newName)
		{
			// Nothing to do
		}
#endif
	};

}

