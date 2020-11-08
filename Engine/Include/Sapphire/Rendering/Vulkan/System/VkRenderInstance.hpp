// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_INSTANCE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_INSTANCE_GUARD

#include <Rendering/Framework/System/IRenderInstance.hpp>

#include <Rendering/Vulkan/System/VkValidationLayers.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class SA_ENGINE_API RenderInstance : public IRenderInstance
	{
		VkInstance mHandle = VK_NULL_HANDLE;

#if SA_VK_VALIDATION_LAYERS

		VkDebugUtilsMessengerEXT debugMessenger = nullptr;

#endif

	public:
		void Create() override final;
		void Destroy() override final;

		static void Init();
		static void UnInit();
	};
}

#endif

#endif // GUARD
