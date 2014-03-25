#pragma once

namespace irr { namespace scene 
{
	class IGeometryCreator;
}}

namespace xihad { namespace render3d
{
	class Geometry
	{
	public:
		typedef irr::scene::IGeometryCreator Creator;

	public:
		static const Creator* creator();
		static void setCreator(const Creator* creator);

	private:
		static const irr::scene::IGeometryCreator* sInstance;
	};
}}
