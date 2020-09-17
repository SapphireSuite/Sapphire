// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/VkDevice.hpp>

#include <Rendering/Vulkan/VkMacro.hpp>
#include <Rendering/Vulkan/VkValidationLayers.hpp>

#include <Rendering/Vulkan/Primitives/VkSwapChain.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	// Device requiered extensions.
	static constexpr const char* requieredExtensions[] =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	constexpr uint32 requieredExtensionsSize = sizeof(requieredExtensions) / sizeof(char*);

	bool CheckExtensionSupport(VkPhysicalDevice _device)
	{
		uint32 extensionCount;
		vkEnumerateDeviceExtensionProperties(_device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(_device, nullptr, &extensionCount, availableExtensions.data());

		// Check each asked supported.
		for (uint32 i = 0; i < requieredExtensionsSize; ++i)
		{
			bool extensionFound = false;

			for (uint32 j = 0; j < availableExtensions.size(); ++j)
			{
				// Extension found.
				if (strcmp(requieredExtensions[i], availableExtensions[j].extensionName) == 0)
				{
					extensionFound = true;
					break;
				}
			}

			// Extension not found.
			if (!extensionFound)
				return false;
		}

		return true;
	}


	const VkQueue& VkDevice::GetPresentQueue() const noexcept
	{
		return mPresentQueue;
	}

	const VkQueue& VkDevice::GetGraphicsQueue() const noexcept
	{
		return mGraphicsQueue;
	}

	const VkQueue& VkDevice::GetTransferQueue() const noexcept
	{
		return mTransferQueue;
	}

	const VkQueueFamilyIndices& VkDevice::GetQueueFamilyIndices() const
	{
		SA_ASSERT(mQueueFamilyIndices.IsCompleted(), InvalidParam, Rendering,
			L"Try to get uncomplete device queue family! Get queue family after device Create().")

		return mQueueFamilyIndices;
	}

	bool VkDevice::IsValid() const noexcept
	{
		return mPhysicalDevice != VK_NULL_HANDLE && mLogicalDevice != VK_NULL_HANDLE;
	}

	void VkDevice::Create(VkPhysicalDevice _device, VkQueueFamilyIndices& _queueFamilyIndices)
	{
		float queuePriority = 1.0f;

		constexpr uint32 queueCreateInfoSize = 3;
		const VkDeviceQueueCreateInfo queueCreateInfos[queueCreateInfoSize]
		{
			// Graphic queue info.
			VkDeviceQueueCreateInfo
			{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,							// sType.
				nullptr,															// pNext.
				0,																	// flags.
				_queueFamilyIndices.graphicsFamily,									// queueFamilyIndex.
				1,																	// queueCount.
				&queuePriority														// pQueuePriorities.
			},

			// Present queue info.
			VkDeviceQueueCreateInfo
			{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,							// sType.
				nullptr,															// pNext.
				0,																	// flags.
				_queueFamilyIndices.presentFamily,									// queueFamilyIndex.
				1,																	// queueCount.
				&queuePriority														// pQueuePriorities.
			},

			// Transfer queue info.
			VkDeviceQueueCreateInfo
			{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,							// sType.
				nullptr,															// pNext.
				0,																	// flags.
				_queueFamilyIndices.transferFamily,									// queueFamilyIndex.
				1,																	// queueCount.
				&queuePriority														// pQueuePriorities.
			}
		};

		const VkPhysicalDeviceFeatures physicalDeviceFeatures
		{
		};

#if SA_VK_VALIDATION_LAYERS

		const VkDeviceCreateInfo deviceCreateInfo
		{
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,					// sType.
			nullptr,												// pNext.
			0,														// flags.
			queueCreateInfoSize,									// queueCreateInfoCount.
			queueCreateInfos,										// pQueueCreateInfo.
			VkValidationLayers::GetLayerNum(),						// enabledLayerCount.
			VkValidationLayers::GetLayerNames(),					// ppEnabledLayerNames.
			GetExtensionNum(),										// enabledExtensionCount.
			GetExtensionNames(),									// ppEnabledExtensionNames.
			&physicalDeviceFeatures									// pEnabledFeatures.
		};

#else

		const VkDeviceCreateInfo deviceCreateInfo
		{
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,					// sType.
			nullptr,												// pNext.
			0,														// flags.
			queueCreateInfoSize,									// queueCreateInfoCount.
			queueCreateInfos,										// pQueueCreateInfo.
			0,														// enabledLayerCount.
			nullptr,												// ppEnabledLayerNames.
			GetExtensionNum(),										// enabledExtensionCount.
			GetExtensionNames(),									// ppEnabledExtensionNames.
			&physicalDeviceFeatures									// pEnabledFeatures.
		};

#endif

		mPhysicalDevice = _device;
		mQueueFamilyIndices = _queueFamilyIndices;

		SA_VK_ASSERT(vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mLogicalDevice),
			CreationFailed,Rendering, L"Failed to create logical device!");

		// Query queues.
		mPresentQueue.Create(*this, _queueFamilyIndices.presentFamily);
		mGraphicsQueue.Create(*this, _queueFamilyIndices.graphicsFamily);
		mTransferQueue.Create(*this, _queueFamilyIndices.transferFamily);
	}

	void VkDevice::Destroy()
	{
		SA_ASSERT(mLogicalDevice != VK_NULL_HANDLE, Nullptr, Rendering, L"Try to destroy null logical device!");
		SA_ASSERT(mPhysicalDevice != VK_NULL_HANDLE, Nullptr, Rendering, L"Try to destroy null physical device!");

		mPresentQueue.Destroy(*this);
		mGraphicsQueue.Destroy(*this);

		vkDestroyDevice(mLogicalDevice, nullptr);

		mLogicalDevice = VK_NULL_HANDLE;
		mPhysicalDevice = VK_NULL_HANDLE;
	}

	uint32 VkDevice::GetExtensionNum() noexcept
	{
		return requieredExtensionsSize;
	}
	const char* const* VkDevice::GetExtensionNames() noexcept
	{
		return requieredExtensions;
	}

	void VkDevice::QueryQueueFamilies(VkPhysicalDevice _device, const VkRenderSurface& _surface, VkQueueFamilyIndices& _queueFamilyIndices)
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, queueFamilies.data());

		for (uint32 i = 0; i < queueFamilies.size(); ++i)
			_queueFamilyIndices.AddFamily(_device, _surface, queueFamilies[i], i);
	}

	bool VkDevice::IsPhysicalDeviceSuitable(VkPhysicalDevice _device, const VkRenderSurface& _surface, VkQueueFamilyIndices& _queueFamilyIndices)
	{
		// Check requiered extensions.
		if (!CheckExtensionSupport(_device))
			return false;


		// Check queue family suitability.
		QueryQueueFamilies(_device, _surface, _queueFamilyIndices);

		if (!_queueFamilyIndices.IsCompleted())
			return false;


		// Graphics SwapChain support.
		if (!VkSwapChain::CheckSupport(_device, _surface))
			return false;


		// Check properties suitability.
		//VkPhysicalDeviceProperties deviceProperties;
		//vkGetPhysicalDeviceProperties(_device, &deviceProperties);

		//VkPhysicalDeviceFeatures deviceFeatures;
		//vkGetPhysicalDeviceFeatures(_device, &deviceFeatures);


		return true;
	}

	VkDevice::operator ::VkDevice() const noexcept
	{
		return mLogicalDevice;
	}

	VkDevice::operator VkPhysicalDevice() const noexcept
	{
		return mPhysicalDevice;
	}
}

#endif
