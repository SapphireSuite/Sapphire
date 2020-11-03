// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_SHADOW_RENDER_PIPELINE_GUARD
#define SAPPHIRE_RENDERING_VK_SHADOW_RENDER_PIPELINE_GUARD

#include <Rendering/Vulkan/Primitives/Pipeline/VkRenderPipeline.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkShadowRenderPipeline : public VkRenderPipeline
	{
	protected:
		void CreateShaderStages(std::vector<VkPipelineShaderStageCreateInfo>& _shaderStages,
			std::vector<VkSpecConstantRange>& _specConstRanges, const PipelineCreateInfos& _infos) override final;

	public:
	};
}

#endif

#endif // GUARD
