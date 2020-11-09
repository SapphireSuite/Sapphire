// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_INSTANCE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_INSTANCE_GUARD

#include <Rendering/Framework/System/IRenderInstance.hpp>

#include <Rendering/Vulkan/System/VkValidationLayers.hpp>
#include <Rendering/Vulkan/System/Device/VkDevice.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class RenderSurface;

	class SA_ENGINE_API RenderInstance : public IRenderInstance
	{
		VkInstance mHandle = VK_NULL_HANDLE;

		Device mDevice;

#if SA_VK_VALIDATION_LAYERS

		VkDebugUtilsMessengerEXT debugMessenger = nullptr;

#endif

	public:
		void SelectDevice(QueueFamilyType _requiredFamilies, const RenderSurface* _surface = nullptr);
		
		void Create() override final;
		void Destroy() override final;

		static void Init();
		static void UnInit();

		operator VkInstance() const noexcept;
	};
}

#endif

#endif // GUARD
