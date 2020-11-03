// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_GPU_STORAGE_BUFFER_BASE_GUARD
#define SAPPHIRE_GPU_STORAGE_BUFFER_BASE_GUARD

#include <Core/Types/IInterface.hpp>

namespace Sa
{
	class IRenderInstance;

	class GPUStorageBufferBase : public IInterface
	{
	public:
		virtual void UpdateData(const IRenderInstance& _instance, uint32 _id, void* _data, uint32 _size, uint32 _offset) = 0;
	};
}

#endif // GUARD
