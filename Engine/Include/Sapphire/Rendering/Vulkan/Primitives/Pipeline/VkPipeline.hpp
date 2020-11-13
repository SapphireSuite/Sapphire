// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_PIPELINE_GUARD
#define SAPPHIRE_RENDERING_VK_PIPELINE_GUARD

#include <Rendering/Framework/Primitives/Pipeline/IPipeline.hpp>

#include <Rendering/APIConfig.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class Device;

	class SA_ENGINE_API Pipeline : public IPipeline
	{
		VkPipeline mHandle = VK_NULL_HANDLE;
		VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;

		VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;


		void CreateDescriptorSetLayout(const Device& _device, const PipelineCreateInfos& _infos);
		void DestroyDescriptorSetLayout(const Device& _device);


		void CreatePipelineLayout(const Device& _device);
		void DestroyPipelineLayout(const Device& _device);

		void CreatePipelineHandle(const Device& _device, const PipelineCreateInfos& _infos);
		void DestroyPipelineHandle(const Device& _device);


		void FillShaderStages(std::vector<VkPipelineShaderStageCreateInfo>& _stages, const std::vector<PipelineShaderInfos>& _shaders);
		
		static void FillVertexBindings(VkPipelineVertexInputStateCreateInfo& _vertexInputInfo, std::unique_ptr<VkVertexInputBindingDescription>& _bindingDesc,
			std::unique_ptr<VkVertexInputAttributeDescription[]>& _attribDescs, const VertexBindingLayout& _vertexBindingLayout) noexcept;
		
		static void FillRasterization(VkPipelineRasterizationStateCreateInfo& _rasterizerInfo, const PipelineRenderModes& _modes) noexcept;

		static void FillRenderPassAttachments(struct RenderPassAttachmentInfos& _renderPassAttInfos, const PipelineCreateInfos& _infos) noexcept;

	public:
		VkPipelineLayout GetLayout() const noexcept;
		VkDescriptorSetLayout GetDescriptorSetLayout() const noexcept;

		void Create(const IRenderInstance& _instance, const PipelineCreateInfos& _infos) override final;
		void Destroy(const IRenderInstance& _instance) override final;

		void Bind(const RenderFrame& _frame) override final;
	};
}

#endif

#endif // GUARD
