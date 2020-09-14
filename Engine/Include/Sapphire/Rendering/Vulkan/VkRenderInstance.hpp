// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_INSTANCE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_INSTANCE_GUARD

#include <Rendering/Framework/IRenderInstance.hpp>

#include <Rendering/Vulkan/VkMacro.hpp>
#include <Rendering/Vulkan/VkDevice.hpp>
#include <Rendering/Vulkan/VkRenderSurface.hpp>


#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkRenderInstance : public IRenderInstance
	{
		struct RenderSurfaceInfos
		{
			const IWindow* window = nullptr;
			VkRenderSurface renderSurface;
		};

		VkDebugUtilsMessengerEXT debugMessenger = nullptr;

		VkInstance mHandle = VK_NULL_HANDLE;

		VkDevice mDevice;

		std::vector<RenderSurfaceInfos> mRenderSurfaceInfos;

		static uint32 sInitCount;

		void Init();
		void UnInit();

		void SelectDevice(const VkRenderSurface& _surface, VkQueueFamilyIndices& _queueFamilyIndices);

		static const std::vector<const char*>& GetRequiredExtensions() noexcept;

	public:
		SA_ENGINE_API void Create() override final;
		SA_ENGINE_API void Destroy() override final;

		SA_ENGINE_API void CreateRenderSurface(const IWindow& _window) override final;
		SA_ENGINE_API void DestroyRenderSurface(const IWindow& _window) override final;

		operator VkInstance() const;
	};
}

#endif

#endif // GUARD
