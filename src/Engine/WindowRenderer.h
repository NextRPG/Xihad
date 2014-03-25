#pragma once
#include "CppBase\ReferenceCounted.h"
#include "Engine\SColor.h"

namespace xihad { namespace ngn
{
	class WindowRenderer : public virtual ReferenceCounted
	{
	public:
		virtual ~WindowRenderer() {}

		virtual void clearBuffer(bool backbuffer, bool zbuffer, const SColor& color) = 0;

		virtual void swapBuffer() = 0;
	};
}}

