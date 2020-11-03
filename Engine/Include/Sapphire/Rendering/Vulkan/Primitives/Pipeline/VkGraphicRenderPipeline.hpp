// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_GRAPHIC_RENDER_PIPELINE_GUARD
#define SAPPHIRE_RENDERING_VK_GRAPHIC_RENDER_PIPELINE_GUARD

#include <Rendering/Vulkan/Primitives/Pipeline/VkRenderPipeline.hpp>

#include <Rendering/Vulkan/Buffer/VkUniformBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkGraphicRenderPipeline : public VkRenderPipeline
	{
	protected:
		VkUniformBuffer mMatConstantUniformBuffer;
		std::vector<VkUniformBuffer> mObjectUniformBuffers;

		void Create_Internal(const IRenderInstance& _instance, const PipelineCreateInfos& _infos) override;


		void CreateDescriptorSetLayout(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos) override final;
		
		void CreateDescriptorPool(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _imageNum) override final;
		
		void CreateDescriptorSets(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _imageNum) override final;

		void CreateDescriptorSetLayoutBindings(const PipelineCreateInfos& _infos,
			std::vector<VkDescriptorSetLayoutBinding>& _layoutBindings) const noexcept;


		void CreateDescriptorPoolSize(const PipelineCreateInfos& _infos, uint32 _imageNum,
			std::vector<VkDescriptorPoolSize>& _poolSizes) const noexcept;

		struct DescriptorInfo
		{
			union
			{
				VkDescriptorBufferInfo buffer;
				VkDescriptorImageInfo image;
			};

			bool bIsImage = false;
			
			DescriptorInfo(VkDescriptorBufferInfo _buffer) noexcept : buffer{ _buffer }, bIsImage{ false } {}
			DescriptorInfo(VkDescriptorImageInfo _image) noexcept : image{ _image }, bIsImage{ true } {}
		};

		void CreateWriteDescriptorSets(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _index,
			std::vector<DescriptorInfo>& _descriptorInfos,
			std::vector<VkWriteDescriptorSet>& _descriptorWrites) const noexcept;

		void CreateUniformBuffers(const VkDevice& _device, const PipelineCreateInfos& _infos, uint32 _imageNum);
		void DestroyUniformBuffers(const VkDevice& _device);

	public:
		void Destroy(const IRenderInstance& _instance) override;

		void InitData(const IRenderInstance& _instance, const void* _data, uint32 _dataSize, uint32 _offset) override final;
		void UpdateData(const IRenderInstance& _instance, const IRenderFrame& _frame, const void* _data, uint32 _dataSize, uint32 _offset) override final;
	};
}

#endif

#endif // GUARD
