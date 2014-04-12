#pragma once
#include "IStackPusherFactory.h"
#include "CTplStackPusher.h"

namespace xihad { namespace particle
{
	template <typename T>
	class TplStackPusherFactory : public IStackPusherFactory
	{
	public:
		virtual IStackPusher* create()
		{
			return new CTplStackPusher<T>(createProduct());
		}

	protected:
		virtual T* createProduct() = 0;
	};
}}

