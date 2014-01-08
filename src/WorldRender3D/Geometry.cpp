#include "Geometry.h"
#include "irrlicht\IGeometryCreator.h"

namespace xihad { namespace ngn
{

	const Geometry::Creator* Geometry::creator()
	{
		return sInstance;
	}

	void Geometry::setCreator( const Creator* creator )
	{
		if (sInstance) sInstance->drop();
		sInstance = creator;
		if (sInstance) sInstance->grab();
	}

	const irr::scene::IGeometryCreator* Geometry::sInstance;
}}

