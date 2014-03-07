#pragma once
#include "CppBase\ReferenceCounted.h"

namespace xihad { namespace ngn
{
	class WindowRenderer : public virtual ReferenceCounted
	{
	public:
		virtual ~WindowRenderer() {}

		virtual void clearBuffer() = 0;

		virtual void swapBuffer() = 0;
	};
}}

