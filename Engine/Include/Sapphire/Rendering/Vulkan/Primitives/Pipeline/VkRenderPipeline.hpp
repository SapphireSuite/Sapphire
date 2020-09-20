// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_PIPELINE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_PIPELINE_GUARD

#include <vulkan/vulkan.h>

#include <Rendering/Config.hpp>

#include <Rendering/Framework/Primitives/Pipeline/IRenderPipeline.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkDevice;

	class VkRenderPipeline : public IRenderPipeline
	{
		VkPipeline mHandle = VK_NULL_HANDLE;
		VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;

		VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;
		VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> mDescriptorSets;

		void Create_Internal(const IRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos);


		virtual void CreateDescriptorSetLayoutBindings(const PipelineCreateInfos& _pipelineInfos,
			std::vector<VkDescriptorSetLayoutBinding>& _layoutBindings) const noexcept;

		void CreateDescriptorSetLayout(const VkDevice& _device, const PipelineCreateInfos& _pipelineInfos);


		virtual void CreateDescriptorPoolSize(const PipelineCreateInfos& _pipelineInfos, uint32 _imageNum,
			std::vector<VkDescriptorPoolSize>& _poolSizes) const noexcept;

		void CreateDescriptorPool(const VkDevice& _device, const PipelineCreateInfos& _pipelineInfos);


		struct DescriptorInfo
		{
			bool bIsImage = false;

			union
			{
				VkDescriptorBufferInfo buffer;
				VkDescriptorImageInfo image;
			};

			DescriptorInfo(VkDescriptorBufferInfo _buffer) noexcept : buffer{ _buffer }, bIsImage{ false } {}
			DescriptorInfo(VkDescriptorImageInfo _image) noexcept : image{ _image }, bIsImage{ true } {}
		};

		virtual void CreateWriteDescriptorSets(const PipelineCreateInfos& _pipelineInfos, uint32 _index,
			std::vector<DescriptorInfo>& _descriptorInfos,
			std::vector<VkWriteDescriptorSet>& _descriptorWrites) const noexcept;

		void CreateDescriptorSets(const VkDevice& _device, const PipelineCreateInfos& _pipelineInfos);


		virtual uint32 GetDescriptorReserveNum(const PipelineCreateInfos& _pipelineInfos) const noexcept;


		void CreateDescriptors(const VkDevice& _device, const PipelineCreateInfos& _pipelineInfos);
		void DestroyDescriptors(const VkDevice& _device);

	public:
		void Create(const IRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos) override final;
		void Destroy(const IRenderInstance& _instance) override final;

		void Bind(const IRenderFrame& _frame) override final;

		// Optimized re-creation.
		void ReCreate(const IRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos);

		operator VkPipeline() const;
		operator VkPipelineLayout() const;
	};
}

#endif

#endif // GUARD
