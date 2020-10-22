// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_PIPELINE_FLAGS_GUARD
#define SAPPHIRE_RENDERING_PIPELINE_FLAGS_GUARD

#include <Core/Misc/Flags.hpp>

namespace Sa
{
	enum class PipelineFlag
	{
		Lighting = 1 << 0,

		IBL = 1 << 1,
	};

	static constexpr Flags<PipelineFlag> defaultPipelineFlags = PipelineFlag::Lighting | PipelineFlag::IBL;
}

#endif // GUARD
