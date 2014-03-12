#include "TextureManager.h"
#include <irrlicht/IVideoDriver.h>

using namespace irr;
namespace xihad { namespace render3d
{
	TextureManager::TextureManager( irr::video::IVideoDriver& driver ) :
		driver(&driver) { }

	TextureManager::~TextureManager() { }

	TextureManager::Texture* TextureManager::getTexture( TexID id )
	{
		return driver->getTexture(id);
	}

	void TextureManager::renameTexture( TexID newId, Texture* tex )
	{
		driver->renameTexture(tex, newId);
	}

	void TextureManager::removeTexture( Texture* tex )
	{
		driver->removeTexture(tex);
	}

	TextureManager::Texture* TextureManager::addRenderTargetTexture(TexID id, 
		unsigned width, unsigned height)
	{
		core::dimension2du size(width, height);
		return driver->addRenderTargetTexture(size, id);
	}

}}

