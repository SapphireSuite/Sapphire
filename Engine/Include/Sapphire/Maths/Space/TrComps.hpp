// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_MATHS_TR_COMPS_GUARD
#define SAPPHIRE_MATHS_TR_COMPS_GUARD

#include <Core/Types/Int.hpp>

namespace Sa
{
	enum class TrComp : uint8
	{
		None = 0,

		Position = 1 << 0,

		Rotation = 1 << 1,

		Scale = 1 << 2,


		// === Groups ===
		PR = Position | Rotation,
		PS = Position | Scale,

		RS = Rotation | Scale,

		PRS = Position | Rotation | Scale,
	};
}

#endif // GUARD
