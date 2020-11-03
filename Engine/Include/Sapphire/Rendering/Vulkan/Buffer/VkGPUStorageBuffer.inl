// Copyright 2020 Sapphire development team. All Rights Reserved.

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	template <typename T>
	bool VkGPUStorageBuffer<T>::IsValid() const noexcept
	{
		return mHandle.IsValid();
	}

	template <typename T>
	void VkGPUStorageBuffer<T>::Remove(const IRenderInstance& _instance, uint32 _id)
	{
		Base::Remove(_instance, _id);

		const VkDevice device = _instance.As<VkRenderInstance>().GetDevice();

		void* bufferData = nullptr;

		// Disable at index.
		vkMapMemory(device, mHandle, sizeof(T) * _id + offsetof(T, bEnabled), sizeof(bool), 0, &bufferData);

		bool enabled = false;
		memcpy(bufferData, &enabled, sizeof(bool));

		vkUnmapMemory(device, mHandle);
	}

	template <typename T>
	void VkGPUStorageBuffer<T>::Create(const IRenderInstance& _instance, uint32 _capacity)
	{
		const VkDevice device = _instance.As<VkRenderInstance>().GetDevice();

		mHandle.Create(device, sizeof(T) * _capacity,
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		InitNewObjects(_instance, 0, _capacity);
	}

	template <typename T>
	void VkGPUStorageBuffer<T>::Destroy(const IRenderInstance& _instance)
	{
		Base::Destroy(_instance);
		
		const VkDevice device = _instance.As<VkRenderInstance>().GetDevice();

		mHandle.Destroy(device);
	}

	template <typename T>
	void VkGPUStorageBuffer<T>::UpdateData(const IRenderInstance& _instance, uint32 _id, void* _data, uint32 _size, uint32 _offset)
	{
		SA_ASSERT(_id < Base::mDeviceSize, OutOfRange, Rendering, _id, 0u, Base::mDeviceSize);
		SA_ASSERT(_data, Nullptr, Rendering, L"Data nullptr!");

		const VkDevice device = _instance.As<VkRenderInstance>().GetDevice();

		mHandle.UpdateData(device, _data, _size, _id * sizeof(T) + _offset);
	}

	template <typename T>
	void VkGPUStorageBuffer<T>::UpdateObject(const IRenderInstance& _instance, uint32 _id, const T& _object)
	{
		SA_ASSERT(_id < Base::mDeviceSize, OutOfRange, Rendering, _id, 0u, Base::mDeviceSize);

		const VkDevice device = _instance.As<VkRenderInstance>().GetDevice();

		mHandle.UpdateData(device, &_object, sizeof(T), _id * sizeof(T));
	}

	template <typename T>
	void VkGPUStorageBuffer<T>::ReAllocate(const IRenderInstance& _instance)
	{
		const VkDevice device = _instance.As<VkRenderInstance>().GetDevice();

		VkBuffer stagingBuffer;
		stagingBuffer.Create(device, mDeviceSize * 2 * sizeof(T),
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		VkBuffer::Copy(device, mHandle, stagingBuffer, mDeviceSize * sizeof(T));

		// Destroy old buffer.
		mHandle.Destroy(device);

		mHandle = stagingBuffer;
	}

	template <typename T>
	void VkGPUStorageBuffer<T>::InitNewObjects(const IRenderInstance& _instance, uint32 _prevSize, uint32 _newSize)
	{
		Base::InitNewObjects(_instance, _prevSize, _newSize);

		const VkDevice device = _instance.As<VkRenderInstance>().GetDevice();

		void* bufferData = nullptr;

		// Disable at index.
		vkMapMemory(device, mHandle, sizeof(T) * _prevSize, sizeof(T) * _newSize, 0, &bufferData);

		bool enabled = false;
		
		// Set bool bEnabled = false for each new object.
		for (uint32 i = _prevSize; i < _newSize; ++i)
			memcpy(static_cast<char*>(bufferData) + i * sizeof(T) + offsetof(T, bEnabled), &enabled, sizeof(bool));

		vkUnmapMemory(device, mHandle);
	}


	template <typename T>
	VkDescriptorBufferInfo VkGPUStorageBuffer<T>::CreateDescriptorBufferInfo() const noexcept
	{
		return mHandle.CreateDescriptorBufferInfo(sizeof(T) * Base::mDeviceSize);
	}

	template <typename T>
	VkWriteDescriptorSet VkGPUStorageBuffer<T>::CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, uint32 _binding) noexcept
	{
		return VkBuffer::CreateWriteDescriptorSet(_descriptorSet, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, _binding);
	}
}

#endif
