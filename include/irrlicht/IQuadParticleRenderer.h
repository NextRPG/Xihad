#pragma once
#include "IParticleRenderer.h"
#include "irrArray.h"

namespace irr { namespace scene
{
	class IQuadParticleRenderer : public IParticleRenderer
	{
	public:
		IQuadParticleRenderer(video::IVideoDriver* driver, core::array<SParticle>* source = 0) :
			IParticleRenderer(driver, source) 
		{
		}

		virtual void addTextureRegion(const core::rectf& region) = 0;
		virtual const core::array<core::rectf>& getTextureRegions() const = 0;
		virtual void removeAllTextureRegions() = 0;

		bool setOrintation(const core::vector3df& vertical, const core::vector3df& horizontal) 
		{
			core::vector3df validVert(vertical);
			if (validateVector(validVert, horizontal))
			{
				Vertical = validVert;
				Horizontal = horizontal;

				Vertical.normalize();
				Horizontal.normalize();
				return true;
			}

			return false;
		}

		const core::vector3df& getVerticalVector() const 
		{
			return Vertical;
		}

		const core::vector3df& getHorizontalVector() const 
		{
			return Horizontal;
		}

	private:
		bool validateVector(core::vector3df& vert, const core::vector3df& hori)
		{
			core::vector3df cross = vert.crossProduct(hori);
			if (cross.equals(core::vector3df(0,0,0)))
				return false;

			vert = hori.crossProduct(cross);
			return true;
		}

	private:
		core::vector3df Vertical;
		core::vector3df Horizontal;
	};
}}

