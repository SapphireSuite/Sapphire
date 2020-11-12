// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_SHADER_STAGE_GUARD
#define SAPPHIRE_RENDERING_SHADER_STAGE_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/APIConfig.hpp>

namespace Sa
{
	enum class ShaderStage : uint8
	{
		Unknown = 0,

		Vertex = 1 << 0,
		Fragment = 1 << 1,
		Geometry = 1 << 2,
		Compute = 1 << 3
	};


#if SA_RENDERING_API == SA_VULKAN

	VkShaderStageFlagBits API_GetShaderStage(ShaderStage _type) noexcept;

	VkShaderStageFlags API_GetShaderStageFlags(ShaderStage _type) noexcept;

#endif

}

#endif // GUARD
