#pragma once
#include "IParticleInitializer.h"
#include "vector3d.h"

namespace irr { namespace scene
{
	class IParticleGeometricInitializer : public IParticleInitializer
	{
	public:
		IParticleGeometricInitializer( f32 minDirSize = 5.f, f32 maxDirSize = 10.f, bool outlineOnly = true);

		virtual void initialize(SParticle&);

		void setMinNormalDirectionSize( f32 minSize ) { MinNormalDirectionSize = minSize; }

		void setMaxNormalDirectionSize( f32 maxSize ) { MaxNormalDirectionSize = maxSize; }

		f32 getMinNormalDirectionSize() const { return MinNormalDirectionSize; }

		f32 getMaxNormalDirectionSize() const { return MaxNormalDirectionSize; }

		bool isUsingNormalDirection() const
		{
			f32 min = getMinNormalDirectionSize();
			f32 max = getMaxNormalDirectionSize();
			return ! (min==max && min==0.f);
		}

		void setOutlineOnly(bool only) { OutlineOnly = only; }

		bool isOutlineOnly() const { return OutlineOnly; }

	protected:
		virtual core::vector3df generatePoint(bool outlineOnly) = 0;

		// No need to be normalized
		virtual core::vector3df generateNormalDirection(const core::vector3df& point) = 0;

	private:
		f32 MinNormalDirectionSize;
		f32 MaxNormalDirectionSize;
		bool OutlineOnly;
	};
}}

