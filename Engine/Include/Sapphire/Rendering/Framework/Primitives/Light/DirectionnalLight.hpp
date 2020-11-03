// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_DIRECTIONNAL_LIGHT_GUARD
#define SAPPHIRE_RENDERING_DIRECTIONNAL_LIGHT_GUARD

#include <Rendering/Framework/Primitives/Light/ILight.hpp>
#include <Rendering/Framework/Primitives/Light/DirectionnalLight_GPU.hpp>

namespace Sa
{
	class DirectionnalLight : public ILight
	{
		Vec3f mDirection;

		bool mDirectionDirty = true;

	public:
		using GPU_T = DirectionnalLight_GPU;

		using ILight::ILight;

		DirectionnalLight(DirectionnalLight&&) = default;
		DirectionnalLight(const DirectionnalLight&) = default;


		const Vec3f& GetDirection() const noexcept;

		void SetDirection(const Vec3f& _direction);

		void Update(const IRenderInstance& _instance, void* _gpuBuffer) override final;

		DirectionnalLight& operator=(DirectionnalLight&& _rhs) noexcept;
		DirectionnalLight& operator=(const DirectionnalLight& _rhs) noexcept;
	};
}

#endif // GUARD
