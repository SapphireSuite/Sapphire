// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_SHADOWMAP_PIPELINE_GUARD
#define SAPPHIRE_RENDERING_VK_SHADOWMAP_PIPELINE_GUARD

#include <Rendering/Vulkan/Primitives/Pipeline/VkRenderPipeline.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkShadowmapPipeline : public VkRenderPipeline
	{
	protected:
		void CreateShaderStages(std::vector<VkPipelineShaderStageCreateInfo>& _shaderStages,
			std::vector<VkSpecConstantRange>& _specConstRanges, const PipelineCreateInfos& _infos) override final;

		void CreateDescriptorSetLayout(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos) override final;

		void CreateDescriptorPool(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _imageNum) override final;

		void CreateDescriptorSets(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _imageNum) override final;

	public:
	};
}

#endif

#endif // GUARD
