#pragma once
#include "TplStackPusherFactory.h"

namespace xihad { namespace particle
{
	template <typename T>
	class DefaultStackPusherFactory : public TplStackPusherFactory<T>
	{
	protected:
		virtual T* createProduct() 
		{
			return new T;
		}
	};
}}

