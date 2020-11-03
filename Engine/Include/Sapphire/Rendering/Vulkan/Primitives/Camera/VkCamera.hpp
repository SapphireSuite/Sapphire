// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_CAMERA_GUARD
#define SAPPHIRE_RENDERING_VK_CAMERA_GUARD

#include <Rendering/Framework/Primitives/Camera/ICamera.hpp>

namespace Sa
{
	class VkCamera : public ICamera
	{
	public:
		using ICamera::ICamera;

		void Update(const IRenderInstance& _instance, void* _gpuBuffer) override final;
	};
}

#endif // GUARD
