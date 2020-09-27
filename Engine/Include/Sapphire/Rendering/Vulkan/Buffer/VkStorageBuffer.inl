// Copyright 2020 Sapphire development team. All Rights Reserved.

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	template <typename T>
	uint32 VkStorageBuffer<T>::Size() const noexcept
	{
		return mSize;
	}

	template <typename T>
	uint32 VkStorageBuffer<T>::Capacity() const noexcept
	{
		return mCapacity;
	}

	template <typename T>
	uint32 VkStorageBuffer<T>::Add(const VkDevice& _device, const T& _object)
	{
		// Buffer not created yet.
		if (!mHandle.IsValid())
			Create(_device);
		else if (mSize + 1 > mCapacity) // Need to re-allocate buffer.
		{
			mCapacity *= 2;

			VkBuffer stagingBuffer;
			stagingBuffer.Create(_device, mCapacity * sizeof(T), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			VkBuffer::Copy(_device, mHandle, stagingBuffer, mSize * sizeof(T));

			// Manually destroy.
			mHandle.Destroy(_device);

			mHandle = stagingBuffer;
		}

		// Increment size before callback event.
		++mSize;
		
		// Update new object.
		UpdateObject(_device, _object, mSize - 1);

		// TODO: Add descriptor callback event.

		return mSize - 1;
	}

	template <typename T>
	void VkStorageBuffer<T>::Remove(const VkDevice& _device, uint32 _id)
	{
		SA_ASSERT(_id < mSize, OutOfRange, Rendering, _id, 0u, mSize);

		void* bufferData = nullptr;

		vkMapMemory(_device, mHandle, sizeof(T) * _id, mSize - _id, 0, &bufferData);

		// Handle input overlapping.
		memmove(bufferData, &bufferData + sizeof(T), (mSize - _id - 1) * sizeof(T));

		vkUnmapMemory(_device, mHandle);

		--mSize;

		// TODO: Add descriptor callback event.
	}

	template <typename T>
	void VkStorageBuffer<T>::Create(const VkDevice& _device, uint32 _capacity)
	{
		mCapacity = _capacity;

		mHandle.Create(_device, sizeof(T) * _capacity, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}

	template <typename T>
	void VkStorageBuffer<T>::Destroy(const VkDevice& _device)
	{
		mSize = mCapacity = 0u;

		mHandle.Destroy(_device);
	}

	template <typename T>
	void VkStorageBuffer<T>::UpdateObject(const VkDevice& _device, const T& _object, uint32 _id)
	{
		SA_ASSERT(_id < mSize, OutOfRange, Rendering, _id, 0u, mSize);

		mHandle.UpdateData(_device, &_object, sizeof(T), _id * sizeof(T));
	}

	template <typename T>
	VkDescriptorBufferInfo VkStorageBuffer<T>::CreateDescriptorBufferInfo() const noexcept
	{
		return mHandle.CreateDescriptorBufferInfo(sizeof(T) * mSize);
	}

	template <typename T>
	VkWriteDescriptorSet VkStorageBuffer<T>::CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, uint32 _binding) noexcept
	{
		return VkBuffer::CreateWriteDescriptorSet(_descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, _binding);
	}
}

#endif
