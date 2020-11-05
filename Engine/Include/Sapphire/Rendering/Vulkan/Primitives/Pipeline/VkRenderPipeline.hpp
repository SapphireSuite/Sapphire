// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_PIPELINE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_PIPELINE_GUARD

#include <Rendering/Config.hpp>

#include <Rendering/Framework/Primitives/Pipeline/IRenderPipeline.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa
{
	class VkDevice;
	class VkRenderInstance;

	struct VkSpecConstantRange;

	class VkRenderPipeline : public IRenderPipeline
	{
	protected:// TODO: REMOVE LATER.
		void CreateDescriptors(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos);
		void DestroyDescriptors(const VkDevice& _device);

	protected:
	public:// TODO: REMOVE LATER.
		VkPipeline mHandle = VK_NULL_HANDLE;
		VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;

		VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;
		VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> mDescriptorSets;


		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API virtual void Create_Internal(const IRenderInstance& _instance, const PipelineCreateInfos& _infos);


		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API virtual void CreateDescriptorSetLayout(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos) = 0;
		
		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API virtual void CreateDescriptorPool(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _imageNum) = 0;
		
		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API virtual void CreateDescriptorSets(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _imageNum) = 0;

		
		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API virtual void CreateShaderStages(std::vector<VkPipelineShaderStageCreateInfo>& _shaderStages,
			std::vector<VkSpecConstantRange>& _specConstRanges, const PipelineCreateInfos& _infos);

		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API virtual VkPipelineViewportStateCreateInfo CreateViewportStateCreateInfos(const PipelineCreateInfos& _infos,
			std::vector<VkViewport>& _viewports, std::vector<VkRect2D>& _scissors);

		// TODO: Fix unique_ptr bug.
		//virtual VkPipelineVertexInputStateCreateInfo CreateVertexInputInfos(const PipelineCreateInfos& _infos);

		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API virtual VkPipelineRasterizationStateCreateInfo CreateRasterizationInfos(const PipelineCreateInfos& _infos);

	public:
		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void Create(const IRenderInstance& _instance, const PipelineCreateInfos& _infos) override final;
		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void Destroy(const IRenderInstance& _instance) override;

		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void Bind(const IRenderFrame& _frame) const override final;

		// Optimized re-creation.
		void ReCreate(const IRenderInstance& _instance, const PipelineCreateInfos& _infos);

		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API operator VkPipeline() const;
		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API operator VkPipelineLayout() const;
	};
}

#endif

#endif // GUARD
