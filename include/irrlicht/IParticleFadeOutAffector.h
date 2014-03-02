// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_PARTICLE_FADE_OUT_AFFECTOR_H_INCLUDED__
#define __I_PARTICLE_FADE_OUT_AFFECTOR_H_INCLUDED__

#include "IParticleAffector.h"

namespace irr
{
namespace scene
{

enum E_FADEOUT_COLOR_MASK
{
	EFCM_RED_ENABLE		= 0x1,
	EFCM_GREEN_ENABLE	= 0x2,
	EFCM_BLUE_ENABLE	= 0x4,
	EFCM_ALPHA_ENABLE	= 0x8,
	EFCM_ALL_ENABLE		= 0xf,
};

//! A particle affector which fades out the particles.
class IParticleFadeOutAffector : public IParticleAffector
{
public:

	//! Sets the targetColor, i.e. the color the particles will interpolate to over time.
	virtual void setTargetColor( const video::SColor& targetColor ) = 0;

	//! Gets the targetColor, i.e. the color the particles will interpolate to over time.
	virtual const video::SColor& getTargetColor() const = 0;

	virtual void setColorMask(E_FADEOUT_COLOR_MASK) = 0;

	virtual E_FADEOUT_COLOR_MASK getColorMask() const = 0;

	virtual bool isChannelEnabled(E_FADEOUT_COLOR_MASK) const = 0;

};

} // end namespace scene
} // end namespace irr


#endif // __I_PARTICLE_FADE_OUT_AFFECTOR_H_INCLUDED__

