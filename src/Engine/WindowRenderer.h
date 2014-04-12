#pragma once
#include "CppBase\ReferenceCounted.h"
#include "Engine\SColor.h"

namespace xihad { namespace ngn
{
	class WindowRenderer : public virtual ReferenceCounted
	{
	public:
		WindowRenderer() { XIHAD_MLD_NEW_OBJECT; }

		virtual ~WindowRenderer() { XIHAD_MLD_DEL_OBJECT; }

		virtual void clearBuffer(bool backbuffer, bool zbuffer, const SColor& color) = 0;

		virtual void swapBuffer() = 0;
	};
}}

