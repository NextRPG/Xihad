#include "CameraRenderTarget.h"
#include <CppBase\xassert.h>

namespace xihad { namespace render3d
{
	using namespace core;
	using namespace video;

	CameraRenderTarget::CameraRenderTarget(ITexture& texture) :
		texture(&texture), target(ERT_RENDER_TEXTURE)
	{
	}

	CameraRenderTarget::CameraRenderTarget(E_RENDER_TARGET target) :
		target(target), texture(0)
	{
		xassert(target != ERT_RENDER_TEXTURE);
	}

	CameraRenderTarget::~CameraRenderTarget() { }

	bool CameraRenderTarget::operator<( const CameraRenderTarget& other ) const
	{
		if (this->target < other.target) return true;

		if (this->texture < other.texture) return true;

		return false;
	}

	bool CameraRenderTarget::operator!=( const CameraRenderTarget& other ) const
	{
		return this->target!=other.target || this->texture!=other.texture;
	}


}}

