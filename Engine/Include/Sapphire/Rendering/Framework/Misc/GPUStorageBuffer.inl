// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename T>
	uint32 GPUStorageBuffer<T>::Add_Internal(const IRenderInstance& _instance, const T& _object)
	{
		const uint32 id = mDeviceSize++;

		UpdateDescriptors();

		return id;
	}

	template <typename T>
	uint32 GPUStorageBuffer<T>::Add(const IRenderInstance& _instance, const T& _object)
	{
		// Buffer not created yet.
		if (!IsValid())
			Create(_instance);
		else if(mDeviceSize >= mDeviceCapacity) // Need to re-allocate buffer.
			ReAllocate(_instance, mDeviceCapacity * 2u);

		return Add_Internal(_instance, _object);
	}

	template <typename T>
	void GPUStorageBuffer<T>::Remove(const IRenderInstance& _instance, uint32 _id)
	{
		SA_ASSERT(_id < mDeviceSize, OutOfRange, Rendering, _id, 0u, mDeviceSize);

		onRemove(_id);

		--mDeviceSize;
		UpdateDescriptors();

		// Child memmove implementation.
	}


	template <typename T>
	void GPUStorageBuffer<T>::Resize(const IRenderInstance& _instance, uint32 _size, const T& _object)
	{
		// Buffer not created yet.
		if (!IsValid())
			Create(_instance);
		else if(_size > mDeviceCapacity)
			ReAllocate(_instance, _size);

		for (uint32 i = 0; i < _size; ++i)
			Add_Internal(_instance, _object);
	}

	template <typename T>
	void GPUStorageBuffer<T>::Reserve(const IRenderInstance& _instance, uint32 _capacity)
	{
		if (mDeviceCapacity >= _capacity)
			return;

		ReAllocate(_instance, _capacity);
	}


	template <typename T>
	void GPUStorageBuffer<T>::ReAllocate(const IRenderInstance& _instance, uint32 _newCapacity)
	{
		mDeviceCapacity = _newCapacity;

		InitNewObjects(_instance);
	}


	template <typename T>
	void GPUStorageBuffer<T>::Create(const IRenderInstance& _instance, uint32 _capacity)
	{
		mDeviceCapacity = _capacity;

		InitNewObjects(_instance);
	}

	template <typename T>
	void GPUStorageBuffer<T>::Destroy(const IRenderInstance& _instance)
	{
		mDeviceCapacity = mDeviceSize = 0u;
	}
}
