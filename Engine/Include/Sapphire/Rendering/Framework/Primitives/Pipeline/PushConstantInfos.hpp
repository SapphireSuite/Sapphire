// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_RENDERING_PUSH_CONSTANT_INFOS_GUARD
#define SAPPHIRE_RENDERING_PUSH_CONSTANT_INFOS_GUARD

#include <vector>

#include <Core/Types/Int.hpp>

#include <Rendering/Framework/Primitives/Shader/ShaderType.hpp>

namespace Sa
{
	struct PushConstantInfo
	{
		uint8 shaderStageFlags = 0u;

		uint32 size = 0u;
	};

	struct PushConstantInfos : public std::vector<PushConstantInfo>
	{
		using vector::vector;

		SA_ENGINE_API static const PushConstantInfos default;

#if SA_RENDERING_API == SA_VULKAN

		void API_GetPushConstantRanges(std::vector<VkPushConstantRange>& _pushConstantRanges) const noexcept;

#endif
	};
}

#endif // GUARD
