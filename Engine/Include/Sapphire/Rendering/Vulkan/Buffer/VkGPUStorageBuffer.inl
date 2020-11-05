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
		const VkDevice device = _instance.As<VkRenderInstance>().GetDevice();

		// Move data.
		void* bufferData = nullptr;
		vkMapMemory(device, mHandle, sizeof(T) * _id, sizeof(T) * (Base::mDeviceSize - _id), 0, &bufferData);

		memmove(bufferData, reinterpret_cast<char*>(bufferData) + sizeof(T), sizeof(T) * (Base::mDeviceSize - _id - 1));

		vkUnmapMemory(device, mHandle);

		Base::Remove(_instance, _id);
	}

	template <typename T>
	void VkGPUStorageBuffer<T>::Create(const IRenderInstance& _instance, uint32 _capacity)
	{
		const VkDevice device = _instance.As<VkRenderInstance>().GetDevice();

		mHandle.Create(device, sizeof(T) * _capacity,
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);


		Base::Create(_instance, _capacity);
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
	void VkGPUStorageBuffer<T>::ReAllocate(const IRenderInstance& _instance, uint32 _newCapacity)
	{
		const VkDevice device = _instance.As<VkRenderInstance>().GetDevice();

		VkBuffer stagingBuffer;
		stagingBuffer.Create(device, _newCapacity * sizeof(T),
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		VkBuffer::Copy(device, mHandle, stagingBuffer, Base::mDeviceCapacity * sizeof(T));

		// Destroy old buffer.
		mHandle.Destroy(device);

		mHandle = stagingBuffer;


		Base::ReAllocate(_instance, _newCapacity);
	}

	template <typename T>
	void VkGPUStorageBuffer<T>::InitNewObjects(const IRenderInstance& _instance)
	{
	}

	template <typename T>
	void VkGPUStorageBuffer<T>::UpdateDescriptors()
	{
		// TODO: Implement.
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
