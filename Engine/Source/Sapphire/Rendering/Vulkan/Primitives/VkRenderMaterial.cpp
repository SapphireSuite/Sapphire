// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/VkRenderMaterial.hpp>

#include <Rendering/Vulkan/Primitives/Pipeline/VkRenderPipeline.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkRenderMaterial::CreatePipeline(const IRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos)
	{
		mPipeline = new VkRenderPipeline;

		mPipeline->Create(_instance, _pipelineInfos);
	}
}

#endif
