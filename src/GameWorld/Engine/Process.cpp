#include "Process.h"
#include "CppBase/xassert.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace xihad { namespace ngn
{
	void Process::sleep( float seconds )
	{
#ifdef _WIN32
		::Sleep(static_cast<DWORD>(seconds*1000));
#else
		xassert(false, "sleep function is not supported on other platform now");
#endif // WIN32
	}

}}
