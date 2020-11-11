// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_QUEUE_MANAGER_GUARD
#define SAPPHIRE_RENDERING_VK_QUEUE_MANAGER_GUARD

#include <Rendering/Vulkan/System/Device/VkQueue.hpp>

namespace Sa::Vk
{
	struct PhysicalDeviceInfos;

	class QueueManager
	{
	public:
		Queue graphics;
		Queue compute;
		Queue transfer;
		Queue present;

		void Create(const Device& _device, const PhysicalDeviceInfos& _infos);
		void Destroy(const Device& _device);
	};
}

#endif // GUARD
