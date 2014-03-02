#pragma once
#include "TplStackPusherFactory.h"
#include <irrlicht/IParticleSystemFactory.h>
#include <irrlicht/smartptr.h>

namespace xihad { namespace particle
{
	template <typename T, T* (irr::scene::IParticleSystemFactory::* method)()>
	class IrrDelegateStackPusherFactory : public TplStackPusherFactory<T>
	{
		typedef irr::scene::IParticleSystemFactory DelegateFactory;

	public:
		IrrDelegateStackPusherFactory(DelegateFactory* f) : mFactory(f) {}

	protected:
		virtual T* createProduct() override
		{
			return (mFactory.get()->*method)();
		}

	private:
		irrptr<DelegateFactory> mFactory;
	};
}}

