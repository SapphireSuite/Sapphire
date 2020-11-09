// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/Device/VkQueueManager.hpp>

#include <Rendering/Vulkan/System/Device/VkDevice.hpp>

namespace Sa::Vk
{
	Queue& QueueManager::GetGraphicsQueue() noexcept
	{
		return mGraphicsQueue;
	}

	const Queue& QueueManager::GetGraphicsQueue() const noexcept
	{
		return mGraphicsQueue;
	}

	Queue& QueueManager::GetComputeQueue() noexcept
	{
		return mComputeQueue;
	}

	const Queue& QueueManager::GetComputeQueue() const noexcept
	{
		return mComputeQueue;
	}

	Queue& QueueManager::GetTransferQueue() noexcept
	{
		return mTransferQueue;
	}

	const Queue& QueueManager::GetTransferQueue() const noexcept
	{
		return mTransferQueue;
	}

	Queue& QueueManager::GetPresentQueue() noexcept
	{
		return mPresentQueue;
	}

	const Queue& QueueManager::GetPresentQueue() const noexcept
	{
		return mPresentQueue;
	}


	void QueueManager::Create(const Device& _device, const PhysicalDeviceInfos& _infos)
	{
		if (_infos.graphics.index != uint32(-1))
			mGraphicsQueue.Create(_device, _infos.graphics.index, _infos.graphics.queueNum);

		if (_infos.compute.index != uint32(-1))
			mComputeQueue.Create(_device, _infos.compute.index, _infos.compute.queueNum);

		if (_infos.transfer.index != uint32(-1))
			mTransferQueue.Create(_device, _infos.transfer.index, _infos.compute.queueNum);

		if (_infos.present.index != uint32(-1))
			mPresentQueue.Create(_device, _infos.present.index, _infos.compute.queueNum);
	}

	void QueueManager::Destroy(const Device& _device)
	{
		if (mGraphicsQueue.IsValid())
			mGraphicsQueue.Destroy(_device);

		if (mComputeQueue.IsValid())
			mComputeQueue.Destroy(_device);

		if (mTransferQueue.IsValid())
			mTransferQueue.Destroy(_device);

		if (mPresentQueue.IsValid())
			mPresentQueue.Destroy(_device);
	}
}