// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_RENDERING_SPEC_CONSTANT_INFOS_GUARD
#define SAPPHIRE_RENDERING_SPEC_CONSTANT_INFOS_GUARD

#include <vector>

#include <Core/Types/Int.hpp>

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

#endif

namespace Sa
{
	struct SpecConstantInfo
	{
		uint32 constantID = 0u;

		uint32 size = 0u;
	};

#if SA_RENDERING_API == SA_VULKAN

	struct VkSpecConstantRange
	{
		std::vector<VkSpecializationMapEntry> entries;
		VkSpecializationInfo infos;
	};

#endif

	struct SpecConstantInfos : public std::vector<SpecConstantInfo>
	{
		// Init in MaterialShader.cpp.
		static SpecConstantInfos defaultVertex;
		static SpecConstantInfos defaultFragment;

		using vector::vector;

#if SA_RENDERING_API == SA_VULKAN

		VkSpecConstantRange& API_GetSpecConstantRanges(std::vector<VkSpecConstantRange>& _specConstantRanges, const void* _data) const noexcept;

#endif
	};

}

#endif // GUARD
