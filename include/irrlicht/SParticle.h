// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __S_PARTICLE_H_INCLUDED__
#define __S_PARTICLE_H_INCLUDED__

#include "vector3d.h"
#include "dimension2d.h"
#include "SColor.h"
#include "CRandomizer.h"

namespace irr
{
namespace scene
{
	//! Struct for holding particle data
	struct SParticle
	{
		SParticle(s32 seed) : randomizer(seed)
		{
		}

		//! Randomized id of the particle
		core::CRandomizer randomizer;

		//! Elapsed time of the particle since born
		f32 localTime;

		//! Length of life time of the particle
		f32 lifeTime;

		//! Position of the particle
		core::vector3df pos;

		//! Direction and speed of the particle
		core::vector3df vector;

		//! Current color of the particle
		video::SColor color;

		//! Scale of the particle.
		/** The current scale of the particle. */
		core::dimension2df size;
	};


} // end namespace scene
} // end namespace irr

#endif

