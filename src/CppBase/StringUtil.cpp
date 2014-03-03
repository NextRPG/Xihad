#include "StringUtil.h"
#include <cstdarg>
#include <string.h>

namespace xihad
{
	int StringUtil::select( const char* target, const char* const options[] )
	{
		for (int i = 0; options[i] != NULL; ++i)
		{			
			if (strcmp(target, options[i]) == 0)
				return i;
		}

		return -1;
	}
}

