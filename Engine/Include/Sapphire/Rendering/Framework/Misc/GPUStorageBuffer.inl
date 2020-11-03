// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename T>
	uint32 GPUStorageBuffer<T>::Size() const noexcept
	{
		return mDeviceSize - SizeOf(mFreeIndices);
	}

	template <typename T>
	uint32 GPUStorageBuffer<T>::Add(const IRenderInstance& _instance, const T& _object)
	{
		// Buffer not created yet.
		if (!IsValid())
			Create(_instance);
		else if (SizeOf(mFreeIndices) != 0)		// Free indices.
		{
			uint32 id = mFreeIndices.back();
			mFreeIndices.pop_back();

			// Update new object.
			UpdateObject(_instance, id, _object);

			return id;
		}


		// Need to re-allocate buffer.
		ReAllocate(_instance);

		uint32 id = mDeviceSize;

		// mDeviceSize + 1: new object will already be updated.
		InitNewObjects(_instance, mDeviceSize + 1, mDeviceSize * 2);

		// Update new object.
		UpdateObject(_instance, id, _object);

		// TODO: Add descriptor callback event.
		
		return id;
	}

	template <typename T>
	void GPUStorageBuffer<T>::Remove(const IRenderInstance& _instance, uint32 _id)
	{
		SA_ASSERT(_id < Size(), OutOfRange, Rendering, _id, 0u, Size());

		// Add free index.
		mFreeIndices.push_back(_id);
	}

	template <typename T>
	void GPUStorageBuffer<T>::Destroy(const IRenderInstance& _instance)
	{
		mDeviceSize = 0u;
		mFreeIndices.clear();
	}

	template <typename T>
	void GPUStorageBuffer<T>::InitNewObjects(const IRenderInstance& _instance, uint32 _prevSize, uint32 _newSize)
	{
		mDeviceSize = _newSize;

		// Add free indices from last to first.
		for (uint32 i = _prevSize; i < _newSize; ++i)
			mFreeIndices.push_back(_newSize - 1 - i);
	}
}
