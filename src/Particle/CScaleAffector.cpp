#include "CScaleAffector.h"

namespace xihad { namespace	 particle
{
	using namespace irr;
	using namespace scene;

	CScaleAffector::CScaleAffector( 
		const dimension2df& scaleTo /*= dimension2df(1.0f, 1.0f)*/ )
		: ScaleTo(scaleTo)
	{

	}

	void CScaleAffector::doAffect( f32 timediff, f32 d, f32 dd, SParticle& particle )
	{
		f32 ratio = dd / (1-d);
		particle.size = particle.size.getInterpolated(ScaleTo, 1-ratio);
	}
}}