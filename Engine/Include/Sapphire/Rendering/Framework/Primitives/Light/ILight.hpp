// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_ILIGHT_GUARD
#define SAPPHIRE_RENDERING_ILIGHT_GUARD

#include <Core/Types/IInterface.hpp>

#include <Maths/Space/Vector3.hpp>

#include <Rendering/Framework/Misc/GPUStorageBufferBase.hpp>

namespace Sa
{
	class IRenderInstance;

	class ILight : public IInterface
	{
		// Storage buffer ID.
		uint32 mID = 0;

		Vec3f mColor = Vec3f::One;

		float mIntensity = 1.0f;

		// Ambient, Diffuse, Specular.
		Vec3f mLightComps = Vec3f(0.0f, 0.65f, 0.35f);

		bool mColorDirty = true;
		bool mIntensityDirty = true;
		bool mLightCompsDirty = true;

	protected:
		template <typename GPU_T>
		void Update_Internal(const IRenderInstance& _instance, GPUStorageBufferBase& _gpuBuffer);

	public:

		ILight(uint32 _ID);
		ILight(ILight&&) = default;
		ILight(const ILight&) = default;


		uint32 ID() const noexcept;

		const Vec3f& GetColor() const noexcept;

		float GetIntensity() const noexcept;

		float GetAmbient() const noexcept;
		float GetDiffuse() const noexcept;
		float GetSpecular() const noexcept;
		const Vec3f& GetLightComponents() const noexcept;


		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void SetColor(const Vec3f& _color);

		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void SetIntensity(float _intensity);

		void SetAmbient(float _ambient);
		void SetDiffuse(float _diffuse);
		void SetSpecular(float _specular);
		void SetLightComponents(const Vec3f& _lightComps);

		void OnGPUObjectRemoved(uint32 _removedID);
		
		virtual void Update(const IRenderInstance& _instance, GPUStorageBufferBase& _gpuBuffer) = 0;

		ILight& operator=(ILight&& _rhs) noexcept;
		ILight& operator=(const ILight& _rhs) noexcept;
	};
}

#include <Rendering/Framework/Primitives/Light/ILight.inl>

#endif // GUARD
