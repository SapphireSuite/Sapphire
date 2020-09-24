// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_ILLUMINATION_MODEL_GUARD
#define SAPPHIRE_RENDERING_ILLUMINATION_MODEL_GUARD

namespace Sa
{
	enum class IlluminationModel
	{
		// Unlit.
		None,

		// Simple Blinn-Phing implementation.
		BlinnPhong,

		// Physically base.
		PBR,
	};
}

#endif // GUARD
