// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_MATERIAL_GUARD
#define SAPPHIRE_RENDERING_VK_MATERIAL_GUARD

#include <Rendering/Framework/Primitives/Material/IMaterial.hpp>

#include <Rendering/APIConfig.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class Device;

	class SA_ENGINE_API Material : public IMaterial
	{
		VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> mDescriptorSets;

		void CreateDescriptorPool(const Device& _device, const MaterialCreateInfos& _infos);
		void DestroyDescriptorPool(const Device& _device);

		void CreateDescriptorSets(const Device& _device, const MaterialCreateInfos& _infos);
		void UpdateDescriptorSets(const Device& _device, const MaterialBindingInfos& _binding);
		void UpdateDescriptorSets(const Device& _device, const std::vector<MaterialBindingInfos>& _bindings);
		void DestroyDescriptorSets(const Device& _device);

	public:
		void Create(const IRenderInstance& _instance, const MaterialCreateInfos& _infos) override final;
		void Destroy(const IRenderInstance& _instance) override final;

		void Bind(const FrameInfos& _frameInfos, const IPipeline& _pipeline) override final;
	};
}

#endif

#endif // GUARD
