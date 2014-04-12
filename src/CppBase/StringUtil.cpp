#include "StringUtil.h"
#include <cstdarg>
#include <string.h>

namespace xihad
{
	int StringUtil::select( const char* target, const char* const options[] )
	{
		for (int i = 0; options[i] != nullptr; ++i)
		{
			if (strcmp(target, options[i]) == 0)
				return i;
		}

		return -1;
	}

	int StringUtil::select( const char* target, 
		const char* const options[], const char* eliminatables )
	{
		for (int i = 0; options[i] != nullptr; ++i)
		{
			if (equal_except(target, options[i], eliminatables))
				return i;
		}

		return -1;
	}

	int StringUtil::equal_except( const char* target, const char* compare, const char* eliminatables )
	{
		if (!target || !compare) return target == compare;

		int i = 0, j = 0;
		while (target[i])
		{
			if (target[i] != compare[j])
			{
				if (!strchr(eliminatables, target[i]))
					return 0;
			}
			else if (compare[j] != '\0')
			{
				++j;
			}

			++i;
		}

		return compare[j] == '\0';
	}

}

