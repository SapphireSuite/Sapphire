// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_SURFACE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_SURFACE_GUARD

#include <Rendering/Framework/System/IRenderSurface.hpp>

#include <Window/IWindow.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

#include <Rendering/Vulkan/System/VkSwapChain.hpp>
#include <Rendering/Vulkan/System/VkRenderPass.hpp>

// DUNNNO
#include <Editor/Editor.hpp>

namespace Sa
{
	class VkRenderSurface : public IRenderSurface
	{
		VkSurfaceKHR mHandle = VK_NULL_HANDLE;

		VkSwapChain mSwapChain;
		edtr::Editor* mEditor = nullptr;
		//TODO aurel: Temporary..
	public:
		std::vector<RenderPass*> mRenderPasses;
	private:
		struct SupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		SupportDetails mSupportDetails;
		
		SupportDetails QuerySupportDetails(VkPhysicalDevice _device) const;
	
	public:
		void InitHandle(VkSurfaceKHR _newHandle, VkDevice _device);
		void UnInitHandle();

		SA_ENGINE_API VkSurfaceFormatKHR ChooseSwapSurfaceFormat() const;
		SA_ENGINE_API VkPresentModeKHR ChooseSwapPresentMode() const;
		SA_ENGINE_API ImageExtent ChooseSwapExtent() const;

		SA_ENGINE_API bool CheckSupport(VkPhysicalDevice _device) const;
		SA_ENGINE_API const SupportDetails& GetSupportDetails() const noexcept;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API VkSwapChain& GetSwapChain();
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API const VkSwapChain& GetSwapChain() const;

		SA_ENGINE_API void Create(const VkDevice& _device, const VkQueueFamilyIndices& _queueFamilyIndices);
		void Destroy(const VkDevice& _device);

		SA_ENGINE_API IRenderPass* CreateRenderPass(const IRenderInstance& _instance, 
													const RenderPassCreateInfos& _createInfos) override final;
		//SA_ENGINE_API void DestroyRenderPass(const IRenderInstance& _instance, IRenderPass& _renderPass) override final;

		SA_ENGINE_API void CreateEditor(const IWindow& _window, const IRenderInstance& _renderInstance);
		
		SA_ENGINE_API void Begin(const VkDevice& _device);
		SA_ENGINE_API void End(const VkDevice& _device);

		SA_ENGINE_API void ResizeCallback(const IRenderInstance& _instance, uint32 _width, uint32 _height) override final;

		operator VkSurfaceKHR() const;
	};
}

#endif

#endif // GUARD
