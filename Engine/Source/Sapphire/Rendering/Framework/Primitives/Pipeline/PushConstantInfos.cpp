// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Pipeline/PushConstantInfos.hpp>

#include <Collections/Debug>

namespace Sa
{
	const PushConstantInfos PushConstantInfos::defaultT
	{
		// Camera index.
		PushConstantInfo{ static_cast<uint8>(ShaderType::Vertex), sizeof(int) }
	};


#if SA_RENDERING_API == SA_VULKAN

	void PushConstantInfos::API_GetPushConstantRanges(std::vector<VkPushConstantRange>& _pushConstantRanges) const noexcept
	{
		uint32 offset = 0u;

		_pushConstantRanges.reserve(size());

		for (auto it = begin(); it != end(); ++it)
		{
			SA_ASSERT(it->size, InvalidParam, Rendering, L"Invalid push constant with size == 0!");

			_pushConstantRanges.emplace_back(VkPushConstantRange
			{
				API_GetShaderFlags(it->shaderStageFlags),				// stageFlags.
				offset,													// offset.
				it->size												// size.
			});

			offset += it->size;
		}
	}

#endif

}
