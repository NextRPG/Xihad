#pragma once

#ifdef _DEBUG

#include <unordered_map>
#include <string>

namespace xihad { namespace ngn
{
	class MemoryLeakDetector
	{
	public:
		static MemoryLeakDetector& getSingleton();
		
		static void releaseSingleton();

		unsigned onObjectCreated(const char* objClassId);

		unsigned onObjectDestroyed(const char* objClassId);

	private:
		MemoryLeakDetector();
		~MemoryLeakDetector();

	private:
		std::unordered_map<std::string, int> objectCounts;

	private:
		static MemoryLeakDetector* sSingleton;
	};
}}

#define XIHAD_MLD_NEW_OBJECT ::xihad::ngn::MemoryLeakDetector::getSingleton().onObjectCreated(__FILE__)
#define XIHAD_MLD_DEL_OBJECT ::xihad::ngn::MemoryLeakDetector::getSingleton().onObjectDestroyed(__FILE__)
#define XIHAD_MLD_RELEASE_INSTANCE ::xihad::ngn::MemoryLeakDetector::releaseSingleton()
#else
#define XIHAD_MLD_NEW_OBJECT
#define XIHAD_MLD_DEL_OBJECT
#define XIHAD_MLD_RELEASE_INSTANCE
#endif
