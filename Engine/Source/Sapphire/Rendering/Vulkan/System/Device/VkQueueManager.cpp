// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/Device/VkQueueManager.hpp>

#include <Collections/Debug>

#include <Rendering/Vulkan/System/Device/VkDevice.hpp>

namespace Sa::Vk
{
	const Queue& QueueManager::GetQueueFromType(QueueFamilyType _type) const
	{
		switch (_type)
		{
			case Sa::Vk::QueueFamilyType::Graphics:
				return graphics;
			case Sa::Vk::QueueFamilyType::Compute:
				return compute;
			case Sa::Vk::QueueFamilyType::Transfer:
				return transfer;
			case Sa::Vk::QueueFamilyType::Present:
				return present;
			default:
				SA_LOG("QueueFamilyType not supported!", Warning, Rendering)
				return graphics;
		}
	}

	void QueueManager::Create(const Device& _device, const PhysicalDeviceInfos& _infos)
	{
		if (_infos.graphics.index != uint32(-1))
			graphics.Create(_device, _infos.graphics.index, _infos.graphics.queueNum);

		if (_infos.compute.index != uint32(-1))
			compute.Create(_device, _infos.compute.index, _infos.compute.queueNum);

		if (_infos.transfer.index != uint32(-1))
			transfer.Create(_device, _infos.transfer.index, _infos.transfer.queueNum);

		if (_infos.present.index != uint32(-1))
			present.Create(_device, _infos.present.index, _infos.present.queueNum);
	}

	void QueueManager::Destroy(const Device& _device)
	{
		if (graphics.IsValid())
			graphics.Destroy(_device);

		if (compute.IsValid())
			compute.Destroy(_device);

		if (transfer.IsValid())
			transfer.Destroy(_device);

		if (present.IsValid())
			present.Destroy(_device);
	}
}