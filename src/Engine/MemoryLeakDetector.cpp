#ifdef _DEBUG

#include "MemoryLeakDetector.h"
#include <iostream>

using namespace std;
namespace xihad { namespace ngn
{
	unsigned MemoryLeakDetector::onObjectCreated( const char* objClassId )
	{
		return ++this->objectCounts[objClassId];
	}

	unsigned MemoryLeakDetector::onObjectDestroyed( const char* objClassId )
	{
		return --this->objectCounts[objClassId];
	}

	MemoryLeakDetector::MemoryLeakDetector()
	{
	}

	MemoryLeakDetector::~MemoryLeakDetector()
	{
		for (auto& kv : objectCounts)
		{
			if (kv.second > 0)
				cerr << "MemoryLeak: " << kv.first << " Rest: " << kv.second << endl;
		}
	}

	MemoryLeakDetector& MemoryLeakDetector::getSingleton()
	{
		if (!sSingleton)
			sSingleton = new MemoryLeakDetector;

		return *sSingleton;
	}

	void MemoryLeakDetector::releaseSingleton()
	{
		delete sSingleton;
		sSingleton = 0;
	}

	MemoryLeakDetector* MemoryLeakDetector::sSingleton = 0;

}}

#endif
