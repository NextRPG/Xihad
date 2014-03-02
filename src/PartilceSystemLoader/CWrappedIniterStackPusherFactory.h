#pragma once
#include "TplStackPusherFactory.h"
#include "CWrappedMeshInitializer.h"
#include "IParticleSystemScriptFactory.h"
#include <irrlicht/smartptr.h>

namespace xihad { namespace particle
{
	template <typename T, T* (irr::scene::IParticleSystemFactory::* method)()>
	class CWrappedIniterStackPusherFactory : 
		public TplStackPusherFactory<CWrappedMeshInitializer>
	{
		typedef irr::scene::IParticleSystemFactory DelegateFactory;

	public:
		CWrappedIniterStackPusherFactory(DelegateFactory* f) : mFactory(f) {}

	protected:
		virtual CWrappedMeshInitializer* createProduct() override
		{
			T* product = (mFactory.get()->*method)();
			CWrappedMeshInitializer* initer = new CWrappedMeshInitializer(product);
			product->drop();
			return initer;
		}

	private:
		irrptr<DelegateFactory> mFactory;
	};
}}

