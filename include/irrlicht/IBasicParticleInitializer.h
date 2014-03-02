#pragma once
#include "IParticleInitializer.h"
#include "SColor.h"
#include "dimension2d.h"

namespace irr { namespace scene
{
	class IBasicParticleInitializer : public IParticleInitializer
	{
	public:
		//! Set minimum starting color for particles
		virtual void setMinStartColor( const video::SColor& color ) = 0;

		//! Set maximum starting color for particles
		virtual void setMaxStartColor( const video::SColor& color ) = 0;

		//! Set the maximum starting size for particles
		virtual void setMaxStartSize( const core::dimension2df& size ) = 0;

		//! Set the minimum starting size for particles
		virtual void setMinStartSize( const core::dimension2df& size ) = 0;

		//! Set the minimum particle life-time in milliseconds
		virtual void setMinLifeTime( u32 lifeTimeMin ) = 0;

		//! Set the maximum particle life-time in milliseconds
		virtual void setMaxLifeTime( u32 lifeTimeMax ) = 0;

		//! Gets the minimum starting color for particles
		virtual const video::SColor& getMinStartColor() const = 0;

		//! Gets the maximum starting color for particles
		virtual const video::SColor& getMaxStartColor() const = 0;

		//! Gets the maximum starting size for particles
		virtual const core::dimension2df& getMaxStartSize() const = 0;

		//! Gets the minimum starting size for particles
		virtual const core::dimension2df& getMinStartSize() const = 0;

		//! Get the minimum particle life-time in milliseconds
		virtual u32 getMinLifeTime() const = 0;

		//! Get the maximum particle life-time in milliseconds
		virtual u32 getMaxLifeTime() const = 0;
	};
}}

