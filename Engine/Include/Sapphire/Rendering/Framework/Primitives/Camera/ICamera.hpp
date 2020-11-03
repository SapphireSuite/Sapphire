// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_ICAMERA_GUARD
#define SAPPHIRE_RENDERING_ICAMERA_GUARD

#include <Core/Types/IInterface.hpp>

#include <Maths/Space/Transform.hpp>

#include <Rendering/Framework/Primitives/Camera/Camera_GPU.hpp>

namespace Sa
{
	class IRenderInstance;

	class ICamera : public IInterface
	{
		TransffPRS mTransf;
		
		float mFOV = 90.0f;
		float mNear = 0.35f;
		float mFar = 25.0f;

		bool mViewDirty = true;
		bool mProjDirty = true;

	protected:
		bool IsViewDirty() const noexcept;
		bool IsProjDirty() const noexcept;

	public:
		using GPU_T = Camera_GPU;

		// Camera storage buffer ID.
		const uint32 ID = 0;

		ICamera(uint32 _ID);
		ICamera(ICamera&&) = default;
		ICamera(const ICamera&) = default;

		const Vec3f& GetPosition() const noexcept;
		const Quatf& GetRotation() const noexcept;
		const Vec3f& GetScale() const noexcept;
		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API const TransffPRS& GetTransform() const noexcept;

		float GetFOV() const noexcept;
		float GetNear() const noexcept;
		float GetFar() const noexcept;

		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void SetPosition(const Vec3f& _position);
		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void SetRotation(const Quatf& _rotation);
		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void SetScale(const Vec3f& _scale);
		// TODO: REMOVE SA_ENGINE_API
		SA_ENGINE_API void SetTransform(const TransffPRS& _tr);

		void SetFOV(float _fov);
		void SetNear(float _near);
		void SetFar(float _far);

		Vec3f ComputeViewPosition() const noexcept;

		Mat4f ComputeInvViewMatrix() noexcept;
		Mat4f ComputeProjMatrix() noexcept;

		virtual void Update(const IRenderInstance& _instance, void* _gpuBuffer) = 0;

		ICamera& operator=(ICamera&&) noexcept;
		ICamera& operator=(const ICamera& _rhs) noexcept;

		bool operator==(const ICamera& _rhs) const noexcept;
		bool operator!=(const ICamera& _rhs) const noexcept;
	};
}

#endif // GUARD
