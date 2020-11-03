// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Collections/Debug>

#include <Rendering/Vulkan/System/VkRenderSurface.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#include <Window/GLFWWindow.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkRenderSurface::InitHandle(VkSurfaceKHR _newHandle, VkDevice _device)
	{
		//SA_ASSERT(mHandle == VK_NULL_HANDLE, AlreadyCreated, Rendering, L"VkSurfaceKHR Handle already created.");

		mHandle = _newHandle;
		if(_device.IsValid())
			mSupportDetails = QuerySupportDetails(_device);
	}

	void VkRenderSurface::UnInitHandle()
	{
		// Assume vkDestroySurfaceKHR has been called before (see VkRenderInstance.DestroyRenderSurface()).
		mHandle = nullptr;
	}

	VkSurfaceFormatKHR VkRenderSurface::ChooseSwapSurfaceFormat() const
	{
		// Prefered
		for (uint32 i = 0; i < mSupportDetails.formats.size(); ++i)
		{
			if (mSupportDetails.formats[i].format == VK_FORMAT_B8G8R8A8_SRGB
				&& mSupportDetails.formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return mSupportDetails.formats[i];
		}

		// Default.
		return mSupportDetails.formats[0];
	}

	VkPresentModeKHR VkRenderSurface::ChooseSwapPresentMode() const
	{
		// Prefered.
		for (uint32 i = 0; i < mSupportDetails.presentModes.size(); ++i)
		{
			if (mSupportDetails.presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
				return mSupportDetails.presentModes[i];
		}

		// Default FIFO always supported.
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	ImageExtent VkRenderSurface::ChooseSwapExtent() const
	{
		if (mSupportDetails.capabilities.currentExtent.width != UINT32_MAX)
			return mSupportDetails.capabilities.currentExtent;
		else
		{
			// TODO: Clean.
			VkExtent2D actualExtent = { 1920, 1080 };

			actualExtent.width = Maths::Max(mSupportDetails.capabilities.minImageExtent.width,
										Maths::Min(mSupportDetails.capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = Maths::Max(mSupportDetails.capabilities.minImageExtent.height,
										Maths::Min(mSupportDetails.capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	VkRenderSurface::SupportDetails VkRenderSurface::QuerySupportDetails(VkPhysicalDevice _device) const
	{
		SupportDetails details{};

		// KHR Capabilities.
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device, mHandle, &details.capabilities);


		// KHR Formats.
		uint32 formatCount = 0u;
		vkGetPhysicalDeviceSurfaceFormatsKHR(_device, mHandle, &formatCount, nullptr);

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(_device, mHandle, &formatCount, details.formats.data());
		}


		// KHR Present Modes.
		uint32 presentModeCount = 0u;
		vkGetPhysicalDeviceSurfacePresentModesKHR(_device, mHandle, &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(_device, mHandle, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	bool VkRenderSurface::CheckSupport(VkPhysicalDevice _device) const
	{
		SupportDetails details = QuerySupportDetails(_device);
		return !details.formats.empty() && !details.presentModes.empty();
	}

	const VkRenderSurface::SupportDetails& VkRenderSurface::GetSupportDetails() const noexcept
	{
		return mSupportDetails;
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

		delete mEditor;

		mSwapChain.Destroy(_device);

		for (size_t i = 0; i < mRenderPasses.size(); ++i)
			delete mRenderPasses[i];
	}

	IRenderPass* VkRenderSurface::CreateRenderPass(const IRenderInstance& _instance, const RenderPassCreateInfos& _createInfos)
	{
		RenderPass* renderPass = mRenderPasses.emplace_back(new RenderPass{}); // TODO THIS BREAK REFERENCES.
		renderPass->Create(_instance, _createInfos);

		return renderPass;
	}

	/*void VkRenderSurface::DestroyRenderPass(const IRenderInstance& _instance, IRenderPass* _renderPass)
	{
		SA_ASSERT(&_renderPass >= mRenderPasses.data() && &_renderPass < mRenderPasses.data() + mRenderPasses.size(),
			OutOfRange, reinterpret_cast<uint64>(&_renderPass), reinterpret_cast<uint64>(mRenderPasses.data()),
			reinterpret_cast<uint64>(mRenderPasses.data() + mRenderPasses.size()));

		_renderPass->Destroy(_instance);
		//mRenderPasses.erase(mRenderPasses.begin() + (&_renderPass - static_cast<IRenderPass**>(mRenderPasses.data()))); // TODO THIS BREAK REFERENCES.
	}*/

	void VkRenderSurface::CreateEditor(const IWindow& _window, const IRenderInstance& _renderInstance)
	{
		RenderPassCreateInfos _createInfo{};
		_createInfo.bClear = false;
		_createInfo.bDepthBuffer = true;
		_createInfo.sampling = SampleBits::Sample8Bits;
		_createInfo.extent = mSwapChain.GetImageExtent();
		_createInfo.format = mSwapChain.GetImageFormat();
		_createInfo.bPresent = true;

		mEditor = new edtr::Editor(_window.As<GLFWWindowT>(), _renderInstance.As<VkRenderInstance>(), *mRenderPasses[0]);
		//mSwapChain.AddRenderPass(mRenderPasses[1]);
	}

	void VkRenderSurface::Begin(const VkDevice& _device)
	{
		mSwapChain.Begin(_device);
		mEditor->StartFrame();
	}

	void VkRenderSurface::End(const VkDevice& _device)
	{
		//mSwapChain.GetRenderFrame().framebuffer->Next();
		mEditor->Draw(mSwapChain.GetRenderFrame());

		mEditor->EndFrame();
		mSwapChain.End(_device);
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
