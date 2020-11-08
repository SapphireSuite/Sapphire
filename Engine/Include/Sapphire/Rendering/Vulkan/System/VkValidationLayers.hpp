// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SA_RENDERING_VK_VALIDATION_LAYERS_GUARD
#define SA_RENDERING_VK_VALIDATION_LAYERS_GUARD

#include <Rendering/Vulkan/System/VkMacro.hpp>

#include <Core/Types/Int.hpp>

#if SA_VK_VALIDATION_LAYERS

namespace Sa::Vk
{
	class ValidationLayers
	{
	public:
		~ValidationLayers() = delete;

		static VkDebugUtilsMessengerCreateInfoEXT GetDebugUtilsMessengerCreateInfo() noexcept;

		static uint32 GetLayerNum() noexcept;
		static const char* const* GetLayerNames() noexcept;

		static bool CheckValidationSupport() noexcept;

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);
	};

}

#endif

#endif // GUARD
