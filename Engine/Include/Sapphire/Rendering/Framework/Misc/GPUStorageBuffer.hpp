// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_GPU_STORAGE_BUFFER_GUARD
#define SAPPHIRE_GPU_STORAGE_BUFFER_GUARD

#include <Collections/Debug>

#include <Core/Algorithms/SizeOf.hpp>
#include <Core/Types/Variadics/Event.hpp>

#include <Rendering/Framework/Misc/GPUStorageBufferBase.hpp>

namespace Sa
{
	template <typename T>
	class GPUStorageBuffer : public GPUStorageBufferBase
	{
	protected:
		// Buffer bound size.
		uint32 mDeviceSize = 0u;

		// Buffer capacity on device.
		uint32 mDeviceCapacity = 0u;

		virtual void ReAllocate(const IRenderInstance& _instance, uint32 _newCapacity);
		virtual void InitNewObjects(const IRenderInstance& _instance) = 0;

		virtual uint32 Add_Internal(const IRenderInstance& _instance, const T& _object);

		virtual void UpdateDescriptors() = 0;

	public:
		Event<void(uint32)> onRemove;

		uint32 Size() const noexcept;
		virtual bool IsValid() const noexcept = 0;

		uint32 Add(const IRenderInstance& _instance, const T& _object = T());
		virtual void Remove(const IRenderInstance& _instance, uint32 _id);

		void Resize(const IRenderInstance& _instance, uint32 _size, const T& _object = T());
		void Reserve(const IRenderInstance& _instance, uint32 _capacity);

		virtual void Create(const IRenderInstance& _instance, uint32 _capacity = 5u);
		virtual void Destroy(const IRenderInstance& _instance);

		virtual void UpdateObject(const IRenderInstance& _instance, uint32 _id, const T& _object) = 0;
	};
}

#include <Rendering/Framework/Misc/GPUStorageBuffer.inl>

#endif // GUARD
