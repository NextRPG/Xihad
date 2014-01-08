#pragma once

namespace luaT
{
	struct ReservedKeyword
	{
		static const char* const __OFFSET;
		static const char* const __UDKEY;

#ifdef _DEBUG
		static const char* const __TYPENAME;
#endif // _DEBUG
	};
}

