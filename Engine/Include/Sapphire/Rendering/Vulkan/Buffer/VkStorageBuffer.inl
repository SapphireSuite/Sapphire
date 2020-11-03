// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename CPU_T, typename GPU_T>
	const VkGPUStorageBuffer<GPU_T>& VkStorageBuffer<CPU_T, GPU_T>::GetGPUBuffer() const noexcept
	{
		return mGPUBuffer;
	}


	template <typename CPU_T, typename GPU_T>
	void VkStorageBuffer<CPU_T, GPU_T>::Create(const VkDevice& _device, uint32 _capacity)
	{
		mCPUBuffer.reserve(_capacity);
		mGPUBuffer.Create(_device, _capacity);
	}
	
	template <typename CPU_T, typename GPU_T>
	void VkStorageBuffer<CPU_T, GPU_T>::Destroy(const VkDevice& _device)
	{
		mCPUBuffer.clear();
		mGPUBuffer.Destroy(_device);
	}


	template <typename CPU_T, typename GPU_T>
	CPU_T& VkStorageBuffer<CPU_T, GPU_T>::Add(const VkDevice& _device, const GPU_T& _object)
	{
		uint32 id = mGPUBuffer.Add(_device, _object);

		return mCPUBuffer.emplace_back(id);
	}
	
	template <typename CPU_T, typename GPU_T>
	void VkStorageBuffer<CPU_T, GPU_T>::Remove(const VkDevice& _device, const CPU_T& _object)
	{
		mGPUBuffer.Remove(_device, _object.ID);

		// O(1) access.
		mCPUBuffer.erase(mCPUBuffer.begin() + (&_object - mCPUBuffer.data()));
	}


	template <typename CPU_T, typename GPU_T>
	void VkStorageBuffer<CPU_T, GPU_T>::Update(const IRenderInstance& _instance)
	{
		for (auto it = mCPUBuffer.begin(); it != mCPUBuffer.end(); ++it)
			it->Update(_instance, &mGPUBuffer);
	}
}
