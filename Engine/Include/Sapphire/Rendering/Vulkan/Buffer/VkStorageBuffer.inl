// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Algorithms/SizeOf.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	template <typename T>
	uint32 VkStorageBuffer<T>::Size() const noexcept
	{
		return mDeviceSize - SizeOf(mFreeIndices);
	}

	template <typename T>
	uint32 VkStorageBuffer<T>::Add(const VkDevice& _device, const T& _object)
	{
		// Buffer not created yet.
		if (!mHandle.IsValid())
			Create(_device);
		else if (SizeOf(mFreeIndices) != 0)		// Free indices.
		{
			uint32 id = mFreeIndices.back();
			mFreeIndices.pop_back();

			// Update new object.
			UpdateObject(_device, _object, id);

			return id;
		}


		// Need to re-allocate buffer.

		VkBuffer stagingBuffer;
		stagingBuffer.Create(_device, mDeviceSize * 2 * sizeof(T), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		VkBuffer::Copy(_device, mHandle, stagingBuffer, mDeviceSize * sizeof(T));

		// Destroy old buffer.
		mHandle.Destroy(_device);

		mHandle = stagingBuffer;

		uint32 id = mDeviceSize;

		// mDeviceSize + 1: new object will already be updated.
		InitNewObjects(_device, mDeviceSize + 1, mDeviceSize * 2);

		// Update new object.
		UpdateObject(_device, _object, id);

		// TODO: Add descriptor callback event.
		
		return id;
	}

	template <typename T>
	void VkStorageBuffer<T>::Remove(const VkDevice& _device, uint32 _id)
	{
		SA_ASSERT(_id < Size(), OutOfRange, Rendering, _id, 0u, Size());

		void* bufferData = nullptr;

		// Disable light at index.
		vkMapMemory(_device, mHandle, sizeof(T) * _id + offsetof(T, bEnabled), sizeof(bool), 0, &bufferData);

		bool enabled = false;
		memcpy(bufferData, &enabled, sizeof(bool));

		vkUnmapMemory(_device, mHandle);


		// Add free index.
		mFreeIndices.push_back(_id);
	}

	template <typename T>
	void VkStorageBuffer<T>::Create(const VkDevice& _device, uint32 _capacity)
	{
		mHandle.Create(_device, sizeof(T) * _capacity, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		InitNewObjects(_device, 0, _capacity);
	}

	template <typename T>
	void VkStorageBuffer<T>::Destroy(const VkDevice& _device)
	{
		mDeviceSize = 0u;
		mFreeIndices.clear();

		mHandle.Destroy(_device);
	}

	template <typename T>
	void VkStorageBuffer<T>::UpdateObject(const VkDevice& _device, const T& _object, uint32 _id)
	{
		SA_ASSERT(_id < mDeviceSize, OutOfRange, Rendering, _id, 0u, mDeviceSize);

		mHandle.UpdateData(_device, &_object, sizeof(T), _id * sizeof(T));
	}

	template <typename T>
	void VkStorageBuffer<T>::InitNewObjects(const VkDevice& _device, uint32 _prevSize, uint32 _newSize)
	{
		mDeviceSize = _newSize;

		void* bufferData = nullptr;

		// Disable light at index.
		vkMapMemory(_device, mHandle, sizeof(T) * _prevSize, sizeof(T) * _newSize, 0, &bufferData);

		bool enabled = false;
		
		for (uint32 i = _prevSize; i < _newSize; ++i)
		{
			// Set bool bEnabled = false for each new object.
			memcpy(static_cast<char*>(bufferData) + i * sizeof(T) + offsetof(T, bEnabled), &enabled, sizeof(bool));

			// Add free indices from last to first.
			mFreeIndices.push_back(_newSize - 1 - i);
		}

		vkUnmapMemory(_device, mHandle);
	}

	template <typename T>
	VkDescriptorBufferInfo VkStorageBuffer<T>::CreateDescriptorBufferInfo() const noexcept
	{
		return mHandle.CreateDescriptorBufferInfo(sizeof(T) * mDeviceSize);
	}

	template <typename T>
	VkWriteDescriptorSet VkStorageBuffer<T>::CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, uint32 _binding) noexcept
	{
		return VkBuffer::CreateWriteDescriptorSet(_descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, _binding);
	}
}

#endif
