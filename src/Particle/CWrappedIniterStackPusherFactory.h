#pragma once
#include "TplStackPusherFactory.h"
#include "IParticleSystemScriptFactory.h"
#include <irrlicht/smartptr.h>

namespace xihad { namespace particle
{
	using namespace irr;
	template <typename WrapT, typename T, T* (scene::IParticleSystemFactory::* method)()>
	class CWrappedIniterStackPusherFactory : 
		public TplStackPusherFactory<WrapT>
	{
		typedef scene::IParticleSystemFactory DelegateFactory;

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

