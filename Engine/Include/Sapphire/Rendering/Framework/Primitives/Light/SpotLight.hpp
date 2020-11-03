// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_SPOT_LIGHT_GUARD
#define SAPPHIRE_RENDERING_SPOT_LIGHT_GUARD

#include <Rendering/Framework/Primitives/Light/ILight.hpp>
#include <Rendering/Framework/Primitives/Light/SpotLight_GPU.hpp>

#include <Maths/Space/Transform.hpp>

namespace Sa
{
	class SpotLight : public ILight
	{
		TransffPR mTransf;

		float mRange = 10.0f;
		float mCutOff = 60.0f;

		bool mTransfDirty = true;
		bool mRangeDirty = true;
		bool mCutOffDirty = true;

	public:
		using GPU_T = SpotLight_GPU;

		using ILight::ILight;

		SpotLight(SpotLight&&) = default;
		SpotLight(const SpotLight&) = default;


		const Vec3f& GetPosition() const noexcept;
		const Quatf& GetRotation() const noexcept;
		const TransffPR& GetTransform() const noexcept;

		float GetRange() const noexcept;
		float GetCutOff() const noexcept;


		void SetPosition(const Vec3f& _position);
		void SetRotation(const Quatf& _rotation);
		void SetTransform(const TransffPR& _transf);

		void SetRange(float _range);
		void SetCutOff(float _cutOff);


		void Update(const IRenderInstance& _instance, GPUStorageBufferBase& _gpuBuffer) override final;


		SpotLight& operator=(SpotLight&& _rhs) noexcept;
		SpotLight& operator=(const SpotLight& _rhs) noexcept;
	};
}

#endif // GUARD
