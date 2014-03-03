#pragma once
#include <boost/intrusive_ptr.hpp>

#define irrptr ::boost::intrusive_ptr
namespace irr 
{
	template <typename T>
	void intrusive_ptr_add_ref(T* obj)
	{
		obj->grab();
	}

	template <typename T>
	void intrusive_ptr_release(T* obj)
	{
		obj->drop();
	}
}

