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


	inline void intrusive_ptr_add_ref(MessageListener* obj)
	{
		obj->grab();
	}

	inline void intrusive_ptr_release(MessageListener* obj)
	{
		obj->drop();
	}
}}