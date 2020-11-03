// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename CPU_T, template<typename> typename GPU_BufferT, typename GPU_T>
	const GPU_BufferT<GPU_T>& StorageBuffer<CPU_T, GPU_BufferT, GPU_T>::GetGPUBuffer() const noexcept
	{
		return mGPUBuffer;
	}


	template <typename CPU_T, template<typename> typename GPU_BufferT, typename GPU_T>
	void StorageBuffer<CPU_T, GPU_BufferT, GPU_T>::Create(const IRenderInstance& _instance, uint32 _capacity)
	{
		mCPUBuffer.reserve(_capacity);
		mGPUBuffer.Create(_instance, _capacity);
	}
	
	template <typename CPU_T, template<typename> typename GPU_BufferT, typename GPU_T>
	void StorageBuffer<CPU_T, GPU_BufferT, GPU_T>::Destroy(const IRenderInstance& _instance)
	{
		mCPUBuffer.clear();
		mGPUBuffer.Destroy(_instance);
	}


	template <typename CPU_T, template<typename> typename GPU_BufferT, typename GPU_T>
	CPU_T& StorageBuffer<CPU_T, GPU_BufferT, GPU_T>::Add(const IRenderInstance& _instance, const GPU_T& _object)
	{
		uint32 id = mGPUBuffer.Add(_instance, _object);

		return mCPUBuffer.emplace_back(id);
	}

	template <typename CPU_T, template<typename> typename GPU_BufferT, typename GPU_T>
	void StorageBuffer<CPU_T, GPU_BufferT, GPU_T>::Remove(const IRenderInstance& _instance, const CPU_T& _object)
	{
		mGPUBuffer.Remove(_instance, _object.ID);

		// O(1) access.
		mCPUBuffer.erase(mCPUBuffer.begin() + (&_object - mCPUBuffer.data()));
	}


	template <typename CPU_T, template<typename> typename GPU_BufferT, typename GPU_T>
	void StorageBuffer<CPU_T, GPU_BufferT, GPU_T>::Update(const IRenderInstance& _instance)
	{
		for (auto it = mCPUBuffer.begin(); it != mCPUBuffer.end(); ++it)
			it->Update(_instance, mGPUBuffer);
	}
}
