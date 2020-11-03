// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_GPU_STORAGE_BUFFER_GUARD
#define SAPPHIRE_GPU_STORAGE_BUFFER_GUARD

#include <Collections/Debug>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Framework/Misc/GPUStorageBufferBase.hpp>

namespace Sa
{
	template <typename T>
	class GPUStorageBuffer : public GPUStorageBufferBase
	{
	protected:
		// Buffer size on device.
		uint32 mDeviceSize = 0u;

		std::vector<uint32> mFreeIndices;

		virtual void ReAllocate(const IRenderInstance& _instance) = 0;
		virtual void InitNewObjects(const IRenderInstance& _instance, uint32 _prevSize, uint32 _newSize);

	public:
		virtual bool IsValid() const noexcept = 0;
		uint32 Size() const noexcept;

		uint32 Add(const IRenderInstance& _instance, const T& _object = T());
		virtual void Remove(const IRenderInstance& _instance, uint32 _id);

		virtual void Create(const IRenderInstance& _instance, uint32 _capacity = 5u) = 0;
		virtual void Destroy(const IRenderInstance& _instance);

		virtual void UpdateData(const IRenderInstance& _instance, uint32 _id, void* _data, uint32 _size, uint32 _offset) = 0;
		virtual void UpdateObject(const IRenderInstance& _instance, uint32 _id, const T& _object) = 0;
	};
}

#include <Rendering/Framework/Misc/GPUStorageBuffer.inl>

#endif // GUARD
