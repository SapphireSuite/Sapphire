// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_POINT_LIGHT_GUARD
#define SAPPHIRE_RENDERING_POINT_LIGHT_GUARD

#include <Rendering/Framework/Primitives/Light/ILight.hpp>
#include <Rendering/Framework/Primitives/Light/PointLight_GPU.hpp>

namespace Sa
{
	class PointLight : public ILight
	{
		Vec3f mPosition;
		float mRange = 10.0f;

		bool mPositionDirty = true;
		bool mRangeDirty = true;

	public:
		using GPU_T = PointLight_GPU;

		using ILight::ILight;

		PointLight(PointLight&&) = default;
		PointLight(const PointLight&) = default;


		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API const Vec3f& GetPosition() const noexcept;

		float GetRange() const noexcept;


		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void SetPosition(const Vec3f& _position);

		void SetRange(float _range);


		void Update(const IRenderInstance& _instance, GPUStorageBufferBase& _gpuBuffer) override final;


		PointLight& operator=(PointLight&& _rhs) noexcept;
		PointLight& operator=(const PointLight& _rhs) noexcept;
	};
}

#endif // GUARD
