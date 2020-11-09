// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/VkRenderSurface.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

namespace Sa::Vk
{
	RenderSurface::RenderSurface(VkSurfaceKHR _handle) noexcept : mHandle{ _handle }
	{
	}

	void RenderSurface::Create(const IRenderInstance& _instance)
	{
		SA_ASSERT(mHandle != VK_NULL_HANDLE, Nullptr, Rendering,
			L"Handle is nullptr. VkSurfaceKHR must be created first: use VkRenderInstance.CreateRenderSurface().");

		mSwapChain.Create(_instance.As<Vk::RenderInstance>().GetDevice(), *this);
	}

	void RenderSurface::Destroy(const IRenderInstance& _instance)
	{
		SA_ASSERT(mHandle != VK_NULL_HANDLE, Nullptr, Rendering,
			L"Handle is nullptr. VkSurfaceKHR must be created first: use VkRenderInstance.CreateRenderSurface().");

		const Device& device = _instance.As<Vk::RenderInstance>().GetDevice();

		mSwapChain.Destroy(device);
	}

	RenderSurface::operator VkSurfaceKHR() const
	{
		return mHandle;
	}
}
