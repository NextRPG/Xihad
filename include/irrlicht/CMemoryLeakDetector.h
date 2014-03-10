#pragma once

#ifdef _DEBUG

#include "irrMap.h"
#include "irrString.h"

namespace irr
{
	class IRRLICHT_API CMemoryLeakDetector
	{
	public:
		static CMemoryLeakDetector& getSingleton();

		static void releaseSingleton();

		void onNamedObjectCreated(const char* objClassId);
		void onNamedObjectDestroy(const char* objClassId);
		void onObjectCreated();
		void onObjectDestroy();

	private:
		CMemoryLeakDetector();
		~CMemoryLeakDetector();

	private:
		core::map<core::stringc, int> NamedObjectCounts;
		int TotalObjectCount;

	private:
		static CMemoryLeakDetector* sSingleton;
	};
}

#define IRR_MLD_NEW_OBJECT ::irr::CMemoryLeakDetector::getSingleton().onObjectCreated()
#define IRR_MLD_DEL_OBJECT ::irr::CMemoryLeakDetector::getSingleton().onObjectDestroy()
#define IRR_MLD_NEW_NAMED_OBJECT ::irr::CMemoryLeakDetector::getSingleton().onNamedObjectCreated(this->getDebugName())
#define IRR_MLD_DEL_NAMED_OBJECT ::irr::CMemoryLeakDetector::getSingleton().onNamedObjectDestroy(this->getDebugName())
#define IRR_MLD_RELEASE_INSTANCE ::irr::CMemoryLeakDetector::releaseSingleton()
#else
#define IRR_MLD_NEW_OBJECT
#define IRR_MLD_DEL_OBJECT
#define IRR_MLD_NEW_NAMED_OBJECT
#define IRR_MLD_DEL_NAMED_OBJECT
#define IRR_MLD_RELEASE_INSTANCE
#endif
