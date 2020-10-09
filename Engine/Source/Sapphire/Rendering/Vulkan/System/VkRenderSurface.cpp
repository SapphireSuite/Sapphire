// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Collections/Debug>

#include <Rendering/Vulkan/System/VkRenderSurface.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkRenderSurface::InitHandle(VkSurfaceKHR _newHandle)
	{
		SA_ASSERT(mHandle == VK_NULL_HANDLE, AlreadyCreated, Rendering, L"VkSurfaceKHR Handle already created.");

		mHandle = _newHandle;
	}

	void VkRenderSurface::UnInitHandle()
	{
		// Assume vkDestroySurfaceKHR has been called before (see VkRenderInstance.DestroyRenderSurface()).
		mHandle = nullptr;
	}

	const ImageExtent& VkRenderSurface::GetImageExtent() const noexcept
	{
		return mSwapChain.GetImageExtent();
	}


	VkSwapChain& VkRenderSurface::GetSwapChain()
	{
		return mSwapChain;
	}

	const VkSwapChain& VkRenderSurface::GetSwapChain() const
	{
		return mSwapChain;
	}

	void VkRenderSurface::Create(const VkDevice& _device, const VkQueueFamilyIndices& _queueFamilyIndices)
	{
		SA_ASSERT(mHandle != VK_NULL_HANDLE, Nullptr, Rendering,
			L"Handle is nullptr. VkSurfaceKHR must be created first: use VkRenderInstance.CreateRenderSurface().");

		mSwapChain.Create(_device, *this, _queueFamilyIndices);
	}

	void VkRenderSurface::Destroy(const VkDevice& _device)
	{
		SA_ASSERT(mHandle != VK_NULL_HANDLE, Nullptr, Rendering,
			L"Handle is nullptr. VkSurfaceKHR must be created first: use VkRenderInstance.CreateRenderSurface().");

		//onResizeEvent.Clear();

		mSwapChain.Destroy(_device);
	}

	IRenderPass& VkRenderSurface::CreateRenderPass(const IRenderInstance& _instance, const RenderPassCreateInfos& _createInfos)
	{
		VkRenderPass& renderPass = mRenderPasses.emplace_back(); // TODO THIS BREAK REFERENCES.
		renderPass.Create(_instance, *this, _createInfos);

		return renderPass;
	}

	void VkRenderSurface::DestroyRenderPass(const IRenderInstance& _instance, IRenderPass& _renderPass)
	{
		SA_ASSERT(&_renderPass >= mRenderPasses.data() && &_renderPass < mRenderPasses.data() + mRenderPasses.size(),
			OutOfRange, reinterpret_cast<uint64>(&_renderPass), reinterpret_cast<uint64>(mRenderPasses.data()),
			reinterpret_cast<uint64>(mRenderPasses.data() + mRenderPasses.size()));

		_renderPass.Destroy(_instance);
		mRenderPasses.erase(mRenderPasses.begin() + (&_renderPass - static_cast<IRenderPass*>(mRenderPasses.data()))); // TODO THIS BREAK REFERENCES.
	}

	void VkRenderSurface::ResizeCallback(const IRenderInstance& _instance, uint32 _width, uint32 _height)
	{
		const ImageExtent& extent = mSwapChain.GetImageExtent();

		// Avoid re-creation on same extent (ex: after window exiting minimization).
		if (extent.width == _width && extent.height == _height)
			return;

		IRenderSurface::ResizeCallback(_instance, _width, _height);

		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		mSwapChain.ReCreate(device, *this, device.GetQueueFamilyIndices());
	}

	VkRenderSurface::operator VkSurfaceKHR() const
	{
		return mHandle;
	}
}

#endif
