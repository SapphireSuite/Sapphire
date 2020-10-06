// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_PIPELINE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_PIPELINE_GUARD

#include <vulkan/vulkan.h>

#include <Rendering/Config.hpp>

#include <Rendering/Framework/Primitives/Pipeline/IRenderPipeline.hpp>

#include <Rendering/Vulkan/Buffer/VkUniformBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkDevice;
	class VkRenderInstance;

	class VkRenderPipeline : public IRenderPipeline
	{
		VkPipeline mHandle = VK_NULL_HANDLE;
		VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;

		VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;
		VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> mDescriptorSets;

		VkUniformBuffer mMatConstantUniformBuffer;
		std::vector<VkUniformBuffer> mObjectUniformBuffers;


		void Create_Internal(const IRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos);


		struct ShaderStageCreateInfos
		{
			std::vector<VkSpecializationMapEntry> specEntries;
			VkSpecializationInfo specInfos;
		};

		void CreateShaderStages(std::vector<VkPipelineShaderStageCreateInfo>& _shaderStages,
			std::vector<ShaderStageCreateInfos>& _stagesInfos, const PipelineCreateInfos& _pipelineInfos);
		

		virtual void CreateDescriptorSetLayoutBindings(const PipelineCreateInfos& _pipelineInfos,
			std::vector<VkDescriptorSetLayoutBinding>& _layoutBindings) const noexcept;

		void CreateDescriptorSetLayout(const VkRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos);


		virtual void CreateDescriptorPoolSize(const PipelineCreateInfos& _pipelineInfos, uint32 _imageNum,
			std::vector<VkDescriptorPoolSize>& _poolSizes) const noexcept;

		void CreateDescriptorPool(const VkRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos, uint32 _imageNum);


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

		virtual void CreateWriteDescriptorSets(const VkRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos, uint32 _index,
			std::vector<DescriptorInfo>& _descriptorInfos,
			std::vector<VkWriteDescriptorSet>& _descriptorWrites) const noexcept;

		void CreateDescriptorSets(const VkRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos, uint32 _imageNum);

		void CreateDescriptors(const VkRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos);
		void DestroyDescriptors(const VkDevice& _device);

		void CreateUniformBuffers(const VkDevice& _device, const PipelineCreateInfos& _pipelineInfos, uint32 _imageNum);
		void DestroyUniformBuffers(const VkDevice& _device);

	public:
		void Create(const IRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos) override final;
		void Destroy(const IRenderInstance& _instance) override final;

		void Bind(const IRenderFrame& _frame) override final;

		void InitData(const IRenderInstance& _instance, const void* _data, uint32 _dataSize, uint32 _offset) override final;
		void UpdateData(const IRenderInstance& _instance, const IRenderFrame& _frame, const void* _data, uint32 _dataSize, uint32 _offset) override final;

		// Optimized re-creation.
		void ReCreate(const IRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos);

		operator VkPipeline() const;
		operator VkPipelineLayout() const;
	};
}

#endif

#endif // GUARD
