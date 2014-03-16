#include "StringConventer.h"
#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <ctype.h>
#else
#include <iostream>
#endif


namespace xihad
{
	void StringConventer::UTF8_to_wstring(const char* utf8_str, std::wstring& out)
	{
#ifdef _WIN32
		int size = MultiByteToWideChar (CP_UTF8, 0, utf8_str, -1, NULL, 0);
		out.resize(size);
		wchar_t* outWchar = const_cast<wchar_t*>(out.data());
		MultiByteToWideChar( CP_UTF8, 0, utf8_str, -1, outWchar, size);
#else
		std::cerr << "String conventer doesn't support this platform yet." << std::endl;
#endif
	}

}

