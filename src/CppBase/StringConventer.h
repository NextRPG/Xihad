#pragma once
#include <string>

namespace xihad
{
	class StringConventer
	{
	public:
		static void UTF8_to_wstring(const char* utf8_str, std::wstring& out);
	};
}

