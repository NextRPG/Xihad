#pragma once
#include <Engine/irr_ptr.h>

namespace irr { namespace video
{
	class IVideoDriver;
	class ITexture;
	enum ECOLOR_FORMAT;
}}

namespace xihad { namespace render3d
{
	class TextureManager
	{
	public:
		typedef const char* TexID;
		typedef irr::video::ITexture Texture;
		typedef irr::video::ECOLOR_FORMAT Format;

	public:
		TextureManager(irr::video::IVideoDriver& driver);
		~TextureManager();

		Texture* getTexture(TexID id);
		void renameTexture(TexID newId, Texture*);
		void removeTexture(Texture*);
		Texture* addRenderTargetTexture(TexID, unsigned width, unsigned height);

	private:
		irr_ptr<irr::video::IVideoDriver> driver;
	};
}}

