// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RAW_SHADER_GUARD
#define SAPPHIRE_RENDERING_RAW_SHADER_GUARD

namespace Sa
{
	struct RawShader : std::vector<char>
	{
		using std::vector<char>::vector;
	};
}

#endif // GUARD
