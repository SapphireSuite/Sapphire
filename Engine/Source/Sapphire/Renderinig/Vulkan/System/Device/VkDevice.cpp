// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/Device/VkDevice.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	void Device::Create(VkPhysicalDevice _device, QueueFamilyIndices& _queueFamilyIndices)
	{
//		float queuePriority = 1.0f;
//
//		constexpr uint32 queueCreateInfoSize = 3;
//		const VkDeviceQueueCreateInfo queueCreateInfos[queueCreateInfoSize]
//		{
//			// Graphic queue info.
//			VkDeviceQueueCreateInfo
//			{
//				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,							// sType.
//				nullptr,															// pNext.
//				0,																	// flags.
//				_queueFamilyIndices.graphicsFamily,									// queueFamilyIndex.
//				1,																	// queueCount.
//				&queuePriority														// pQueuePriorities.
//			},
//
//			// Present queue info.
//			VkDeviceQueueCreateInfo
//			{
//				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,							// sType.
//				nullptr,															// pNext.
//				0,																	// flags.
//				_queueFamilyIndices.presentFamily,									// queueFamilyIndex.
//				1,																	// queueCount.
//				&queuePriority														// pQueuePriorities.
//			},
//
//			// Transfer queue info.
//			VkDeviceQueueCreateInfo
//			{
//				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,							// sType.
//				nullptr,															// pNext.
//				0,																	// flags.
//				_queueFamilyIndices.transferFamily,									// queueFamilyIndex.
//				1,																	// queueCount.
//				&queuePriority														// pQueuePriorities.
//			}
//		};
//
//		VkPhysicalDeviceFeatures physicalDeviceFeatures
//		{
//		};
//		physicalDeviceFeatures.sampleRateShading = VK_TRUE;
//		physicalDeviceFeatures.samplerAnisotropy = VK_TRUE;
//
//#if SA_VK_VALIDATION_LAYERS
//
//		const VkDeviceCreateInfo deviceCreateInfo
//		{
//			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,					// sType.
//			nullptr,												// pNext.
//			0,														// flags.
//			queueCreateInfoSize,									// queueCreateInfoCount.
//			queueCreateInfos,										// pQueueCreateInfo.
//			VkValidationLayers::GetLayerNum(),						// enabledLayerCount.
//			VkValidationLayers::GetLayerNames(),					// ppEnabledLayerNames.
//			GetExtensionNum(),										// enabledExtensionCount.
//			GetExtensionNames(),									// ppEnabledExtensionNames.
//			&physicalDeviceFeatures									// pEnabledFeatures.
//		};
//
//#else
//
//		const VkDeviceCreateInfo deviceCreateInfo
//		{
//			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,					// sType.
//			nullptr,												// pNext.
//			0,														// flags.
//			queueCreateInfoSize,									// queueCreateInfoCount.
//			queueCreateInfos,										// pQueueCreateInfo.
//			0,														// enabledLayerCount.
//			nullptr,												// ppEnabledLayerNames.
//			GetExtensionNum(),										// enabledExtensionCount.
//			GetExtensionNames(),									// ppEnabledExtensionNames.
//			&physicalDeviceFeatures									// pEnabledFeatures.
//		};
//
//#endif
//
//		mPhysicalDevice = _device;
//		mQueueFamilyIndices = _queueFamilyIndices;
//
//		SA_VK_ASSERT(vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mLogicalDevice),
//			CreationFailed, Rendering, L"Failed to create logical device!");
//
//		// Query queues.
//		mPresentQueue.Create(*this, _queueFamilyIndices.presentFamily);
//		mGraphicsQueue.Create(*this, _queueFamilyIndices.graphicsFamily);
//		mTransferQueue.Create(*this, _queueFamilyIndices.transferFamily);
	}

	void Device::Destroy()
	{
		//SA_ASSERT(mLogicalDevice != VK_NULL_HANDLE, Nullptr, Rendering, L"Try to destroy null logical device!");
		//SA_ASSERT(mPhysicalDevice != VK_NULL_HANDLE, Nullptr, Rendering, L"Try to destroy null physical device!");

		//mPresentQueue.Destroy(*this);
		//mGraphicsQueue.Destroy(*this);
		//mTransferQueue.Destroy(*this);

		//vkDestroyDevice(mLogicalDevice, nullptr);

		//mLogicalDevice = VK_NULL_HANDLE;
		//mPhysicalDevice = VK_NULL_HANDLE;
	}

	Device::operator VkDevice() const noexcept
	{
		return mLogicalDevice;
	}

	Device::operator VkPhysicalDevice() const noexcept
	{
		return mPhysicalDevice;
	}
}

#endif
