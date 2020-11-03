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
		void CreateDescriptors(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos);
		void DestroyDescriptors(const VkDevice& _device);

	protected:
		VkPipeline mHandle = VK_NULL_HANDLE;
		VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;

		VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;
		VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> mDescriptorSets;


		virtual void Create_Internal(const IRenderInstance& _instance, const PipelineCreateInfos& _infos);


		virtual void CreateDescriptorSetLayout(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos) = 0;
		
		virtual void CreateDescriptorPool(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _imageNum) = 0;
		
		virtual void CreateDescriptorSets(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _imageNum) = 0;

		
		virtual void CreateShaderStages(std::vector<VkPipelineShaderStageCreateInfo>& _shaderStages,
			std::vector<VkSpecConstantRange>& _specConstRanges, const PipelineCreateInfos& _infos);

		virtual VkPipelineViewportStateCreateInfo CreateViewportStateCreateInfos(const PipelineCreateInfos& _infos,
			std::vector<VkViewport>& _viewports, std::vector<VkRect2D>& _scissors);

		// TODO: Fix unique_ptr bug.
		//virtual VkPipelineVertexInputStateCreateInfo CreateVertexInputInfos(const PipelineCreateInfos& _infos);

		virtual VkPipelineRasterizationStateCreateInfo CreateRasterizationInfos(const PipelineCreateInfos& _infos);

	public:
		void Create(const IRenderInstance& _instance, const PipelineCreateInfos& _infos) override final;
		void Destroy(const IRenderInstance& _instance) override;

		void Bind(const IRenderFrame& _frame) const override final;

		// Optimized re-creation.
		void ReCreate(const IRenderInstance& _instance, const PipelineCreateInfos& _infos);

		operator VkPipeline() const;
		operator VkPipelineLayout() const;
	};
}

#endif

#endif // GUARD
