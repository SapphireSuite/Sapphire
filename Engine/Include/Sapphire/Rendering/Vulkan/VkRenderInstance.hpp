// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_INSTANCE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_INSTANCE_GUARD

#include <Rendering/Framework/IRenderInstance.hpp>

#include <Rendering/Vulkan/VkMacro.hpp>


#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkRenderInstance : public IRenderInstance
	{
		VkDebugUtilsMessengerEXT debugMessenger = nullptr;

		VkInstance mHandle = VK_NULL_HANDLE;

		static uint32 sInitCount;

		void Init();
		void UnInit();

		static const std::vector<const char*>& GetRequiredExtensions() noexcept;

	public:
		SA_ENGINE_API void Create() override final;
		SA_ENGINE_API void Destroy() override final;
	};
}

#endif

#endif // GUARD
