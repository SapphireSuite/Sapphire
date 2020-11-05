// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_SHADOW_CUBEMAP_PIPELINE_GUARD
#define SAPPHIRE_RENDERING_VK_SHADOW_CUBEMAP_PIPELINE_GUARD

#include <Rendering/Vulkan/Primitives/Pipeline/VkRenderPipeline.hpp>

#include <Maths/Space/Matrix4.hpp>

#include <Rendering/Vulkan/Buffer/VkUniformBuffer.hpp>
#include <Rendering/Vulkan/Buffer/VkGPUStorageBuffer.hpp>
#include <Rendering/Vulkan/Buffer/VkImageBuffer.hpp>
#include <Rendering/Vulkan/Buffer/VkCommandBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkShadowCubemapPipeline : public VkRenderPipeline
	{
	protected:
	public:// TODO: REMOVE LATER.
		VkUniformBuffer mLightViewUniformBuffer;
		VkGPUStorageBuffer<Mat4f> mModelUniformBuffer;

		// TODO: CLEAN.
		VkImageBuffer mDepthCubemap;
		VkFramebuffer mFramebuffer = VK_NULL_HANDLE;
		CommandBuffer mPrimaryCommandBuffer;

		VkRenderPass mRenderPass = VK_NULL_HANDLE;

		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void Begin();
		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void Next();
		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void End();


		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void Create_Internal(const IRenderInstance& _instance, const PipelineCreateInfos& _infos) override;


		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void CreateShaderStages(std::vector<VkPipelineShaderStageCreateInfo>& _shaderStages,
			std::vector<VkSpecConstantRange>& _specConstRanges, const PipelineCreateInfos& _infos) override final;

		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void CreateDescriptorSetLayout(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos) override final;

		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void CreateDescriptorPool(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _imageNum) override final;

		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void CreateDescriptorSets(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _imageNum) override final;

		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void CreateUniformBuffers(const IRenderInstance& _instance);
		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void DestroyUniformBuffers(const IRenderInstance& _instance);

	public:
		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void Destroy(const IRenderInstance& _instance) override;

		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void InitData(const IRenderInstance& _instance, const void* _data, uint32 _dataSize, uint32 _offset) override final;
		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void UpdateData(const IRenderInstance& _instance, const IRenderFrame& _frame, const void* _data, uint32 _dataSize, uint32 _offset) override final;
	};
}

#endif

#endif // GUARD
