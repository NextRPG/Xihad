#pragma once
#include <irrlicht/rect.h>
#include <Engine/irr_ptr.h>
#include <irrlicht/IVideoDriver.h>

namespace irr { namespace video
{
	class ITexture;
	enum E_RENDER_TARGET;
}}

namespace xihad { namespace render3d
{
	using namespace irr;
	class CameraRenderTarget
	{
	public:
		typedef video::ITexture RenderTexture;
		typedef core::rectf Viewport;

	public:
		CameraRenderTarget(
			video::E_RENDER_TARGET = video::ERT_FRAME_BUFFER);

		CameraRenderTarget(RenderTexture& tex);
		
		~CameraRenderTarget();

		bool operator!=(const CameraRenderTarget& other) const;

		bool operator<(const CameraRenderTarget& other) const;

		video::E_RENDER_TARGET getType() const { return target; }

		RenderTexture* getRenderTexture() const { return texture.get(); }

		core::recti getAbsoluteViewport(const core::dimension2du& size) const;

	private:
		video::E_RENDER_TARGET target;
		irr_ptr<RenderTexture> texture; 
	};
}}

