// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Shader/SpecConstantInfos.hpp>

#include <Core/Algorithms/SizeOf.hpp>

namespace Sa
{
	// Init in MaterialShader.cpp.
	//SpecConstantInfos SpecConstantInfos::defaultVertex;
	//SpecConstantInfos SpecConstantInfos::defaultFragment;

#if SA_RENDERING_API == SA_VULKAN

	VkSpecConstantRange& SpecConstantInfos::API_GetSpecConstantRanges(std::vector<VkSpecConstantRange>& _specConstantRanges, const void* _data) const noexcept
	{
		VkSpecConstantRange& specConstRange = _specConstantRanges.emplace_back();

		uint32 offset = 0u;
		uint32 totalSize= 0u;
		specConstRange.entries.reserve(size());

		for (auto it = begin(); it != end(); ++it)
		{
			specConstRange.entries.emplace_back(VkSpecializationMapEntry
			{
				it->constantID,						// constantID.
				offset,								// offset.
				it->size							// size.
			});

			offset += it->size;
			totalSize += it->size;
		}

		specConstRange.infos = VkSpecializationInfo
		{
			SizeOf(specConstRange.entries),			// mapEntryCount.
			specConstRange.entries.data(),			// pMapEntries.
			totalSize,								// dataSize.
			_data									// pData.
		};

		return specConstRange;
	}

#endif

}
