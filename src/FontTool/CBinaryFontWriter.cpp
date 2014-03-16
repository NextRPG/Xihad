#include "CBinaryFontWriter.h"
#include <irrlicht\IWriteFile.h>

namespace xihad { namespace font
{
	bool CBinaryFontWriter::write( const SFontInfo& fontInfo )
	{
		// write image
		io::IWriteFile* writer;
		u32 size = fontInfo.charMap.size();
		writer->write(&size, sizeof(u32));
		for (auto& chToarea : fontInfo.charMap)
			writer->write(&chToarea, sizeof(chToarea));


	}

}}

