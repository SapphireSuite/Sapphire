// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Camera/Camera.hpp>

#include <Rendering/Misc/APISpecific.hpp>

namespace Sa
{
	Camera::Camera(uint32 _ID) : ID{ _ID }
	{
	}


	bool Camera::IsViewDirty() const noexcept
	{
		return mViewDirty;
	}
	
	bool Camera::IsProjDirty() const noexcept
	{
		return mProjDirty;
	}


	const Vec3f& Camera::GetPosition() const noexcept
	{
		return mTransf.position;
	}
	
	const Quatf& Camera::GetRotation() const noexcept
	{
		return mTransf.rotation;
	}
	
	const Vec3f& Camera::GetScale() const noexcept
	{
		return mTransf.scale;
	}

	const TransffPRS& Camera::GetTransform() const noexcept
	{
		return mTransf;
	}


	float Camera::GetFOV() const noexcept
	{
		return mFOV;
	}

	float Camera::GetNear() const noexcept
	{
		return mNear;
	}

	float Camera::GetFar() const noexcept
	{
		return mFar;
	}


	void Camera::SetPosition(const Vec3f& _position)
	{
		if (mTransf.position == _position)
			return;

		mViewDirty = true;
		mTransf.position = _position;
	}
	
	void Camera::SetRotation(const Quatf& _rotation)
	{
		if (mTransf.rotation == _rotation)
			return;

		mViewDirty = true;
		mTransf.rotation = _rotation;
	}
	
	void Camera::SetScale(const Vec3f& _scale)
	{
		if (mTransf.scale == _scale)
			return;

		mViewDirty = true;
		mTransf.scale = _scale;
	}
	
	void Camera::SetTransform(const TransffPRS& _tr)
	{
		if (mTransf == _tr)
			return;

		mViewDirty = true;
		mTransf = _tr;
	}


	void Camera::SetFOV(float _fov)
	{
		if (Maths::Equals(mFOV, _fov))
			return;

		mProjDirty = true;
		mFOV = _fov;
	}

	void Camera::SetNear(float _near)
	{
		if (Maths::Equals(mNear, _near))
			return;

		mProjDirty = true;
		mNear = _near;
	}

	void Camera::SetFar(float _far)
	{
		if (Maths::Equals(mFar, _far))
			return;

		mProjDirty = true;
		mFar = _far;
	}


	Vec3f Camera::ComputeViewPosition() const noexcept
	{
		return API_ConvertCoordinateSystem(mTransf.position);
	}

	Mat4f Camera::ComputeViewMatrix() noexcept
	{
		mViewDirty = false;

		return API_ConvertCoordinateSystem(mTransf.Matrix());
	}

	Mat4f Camera::ComputeProjMatrix() noexcept
	{
		mProjDirty = false;

		// TODO: ADD ASPECT.
		return API_ConvertCoordinateSystem(Mat4f::MakePerspective(mFOV, 1200.0f / 800.0f, mNear, mFar));
	}


	Camera& Camera::operator=(Camera&& _rhs) noexcept
	{
		mTransf = Move(_rhs.mTransf);
		mFOV = _rhs.mFOV;
		mNear = _rhs.mNear;
		mFar = _rhs.mFar;

		mViewDirty = true;
		mProjDirty = true;

		return *this;
	}

	Camera& Camera::operator=(const Camera& _rhs) noexcept
	{
		mTransf = _rhs.mTransf;
		mFOV = _rhs.mFOV;
		mNear = _rhs.mNear;
		mFar = _rhs.mFar;

		mViewDirty = true;
		mProjDirty = true;

		return *this;
	}


	bool Camera::operator==(const Camera& _rhs) const noexcept
	{
		return ID == _rhs.ID;
	}

	bool Camera::operator!=(const Camera& _rhs) const noexcept
	{
		return !(*this == _rhs);
	}
}
