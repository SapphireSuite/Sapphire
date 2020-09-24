// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_ALPHA_MODEL_GUARD
#define SAPPHIRE_RENDERING_ALPHA_MODEL_GUARD

namespace Sa
{
	enum class AlphaModel
	{
		// Fully opaque.
		Opaque,

		// Allows transparency (defined in material).
		Transparent,
	};
}

#endif // GUARD
