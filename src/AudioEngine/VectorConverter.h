#pragma once
#include <irrKlang/ik_vec3d.h>
#include "Engine/vector3d.h"

namespace xihad { namespace audio
{
	using namespace irrklang;

	vec3df to_irrklang_vector3d(const ngn::vector3df& vec)
	{
		return reinterpret_cast<const vec3df&>(vec);
	}
}}

