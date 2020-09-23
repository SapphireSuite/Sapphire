// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Collections/Debug>

#include <Rendering/Vulkan/Buffer/VkUniformBuffer.hpp>

#include <Rendering/Vulkan/System/VkDevice.hpp>

namespace Sa
{
	void VkUniformBuffer::UpdateData(const VkDevice& _device, const void* _data, uint32 _size, uint32 _offset)
	{
		SA_ASSERT(_data, Nullptr, Rendering, L"Update nullptr data!")

		void* bufferData;
		
		vkMapMemory(_device, mDeviceMemory, _offset, _size, 0, &bufferData);
		
		memcpy(bufferData, _data, _size);
		
		vkUnmapMemory(_device, mDeviceMemory);
	}
}
