#pragma once
#include "TplStackPusherFactory.h"
#include "IParticleSystemScriptFactory.h"
#include <irrlicht/smartptr.h>

namespace xihad { namespace particle
{
	template <typename WrapT, typename T, T* (irr::scene::IParticleSystemFactory::* method)()>
	class CWrappedIniterStackPusherFactory : 
		public TplStackPusherFactory<WrapT>
	{
		typedef irr::scene::IParticleSystemFactory DelegateFactory;

	public:
		CWrappedIniterStackPusherFactory(DelegateFactory* f) : mFactory(f) {}

	protected:
		virtual WrapT* createProduct() override
		{
			T* product = (mFactory.get()->*method)();
			WrapT* initer = new WrapT(product);
			product->drop();
			return initer;
		}

	private:
		irrptr<DelegateFactory> mFactory;
	};
}}

