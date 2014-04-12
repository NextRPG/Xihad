#pragma once
#include "CppBase/ReferenceCounted.h"
#include "MessageParam.h"

namespace xihad { namespace ngn
{
	class GameObject;
	class MessageListener : public virtual ReferenceCounted
	{
	public:
		typedef MessageParam Parameter;

	public:
		virtual ~MessageListener() {} 
		virtual void receive(GameObject& source, const Parameter&) = 0;
	};
}}