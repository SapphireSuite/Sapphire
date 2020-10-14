// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_CAMERA_GUARD
#define SAPPHIRE_RENDERING_VK_CAMERA_GUARD

#include <Rendering/Framework/Primitives/Camera/ICamera.hpp>

#include <Rendering/Vulkan/Buffer/VkUniformBuffer.hpp>

namespace Sa
{
	class VkCamera : public ICamera
	{
		std::vector<VkUniformBuffer> mUniformBuffers;

	public:
		using ICamera::ICamera;

		~VkCamera() = default;

		// TODO: Remove later.
		SA_ENGINE_API VkUniformBuffer& GetUniformBuffer(uint32 _index);
		const VkUniformBuffer& GetUniformBuffer(uint32 _index) const;
		const std::vector<VkUniformBuffer>& GetUniformBuffers() const;

		VkDescriptorBufferInfo CreateDescriptorBufferInfo(uint32 _index)	const noexcept;
		
		// TODO: Remove later.
		SA_ENGINE_API void Create(const IRenderInstance& _instance, const IRenderSurface& _surface) override final;
		// TODO: Remove later.
		SA_ENGINE_API void Destroy(const IRenderInstance& _instance) override final;
	};
}

#endif // GUARD
