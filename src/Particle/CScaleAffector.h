#pragma once
#include <irrlicht\IParticleAffector.h>

namespace xihad { namespace	 particle
{
	using irr::core::dimension2df;
	class CScaleAffector : public irr::scene::IParticleAffector
	{
	public:
		CScaleAffector(const dimension2df& scaleTo = dimension2df(1.0f, 1.0f));
		
		void setTargetSize(float x, float y)
		{
			ScaleTo = dimension2df(x, y);
		}

		virtual ~CScaleAffector() {}
		
	protected:
		virtual void doAffect( 
			irr::f32 timediff, irr::f32 d, irr::f32 dd, irr::scene::SParticle& particle );
	
	private:
		dimension2df ScaleTo;
	};
}}