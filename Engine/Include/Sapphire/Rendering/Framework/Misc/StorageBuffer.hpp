// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_STORAGE_BUFFER_GUARD
#define SAPPHIRE_RENDERING_STORAGE_BUFFER_GUARD

//#include <Rendering/Vulkan/Buffer/GPUStorageBuffer.hpp>

#include <vector>

#include <Core/Types/Int.hpp>

namespace Sa
{
	class IRenderInstance;

	/**
	*	CPU_T must have "uint32 ID" member.
	*	GPU_T must have "bool bEnabled" member.
	*/
	template <typename CPU_T, template<typename> typename GPU_BufferT, typename GPU_T = typename CPU_T::GPU_T>
	class StorageBuffer
	{
		// CPU-side buffer.
		std::vector<CPU_T> mCPUBuffer;

		// GPU-side buffer.
		GPU_BufferT<GPU_T> mGPUBuffer;

	public:
		const GPU_BufferT<GPU_T>& GetGPUBuffer() const noexcept;

		void Create(const IRenderInstance& _instance, uint32 _capacity = 5u);
		void Destroy(const IRenderInstance& _instance);

		CPU_T& Add(const IRenderInstance& _instance, const GPU_T& _object = GPU_T());
		void Remove(const IRenderInstance& _instance, const CPU_T& _object);

		void Update(const IRenderInstance& _instance);
	};
}

#include <Rendering/Framework/Misc/StorageBuffer.inl>

#endif // GUARD
