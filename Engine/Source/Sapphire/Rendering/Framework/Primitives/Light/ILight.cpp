// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Light/ILight.hpp>

// TODO: REMOVE.
#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/Buffer/VkGPUStorageBuffer.hpp>

namespace Sa
{
	ILight::ILight(uint32 _ID) : ID{ _ID }
	{
	}


	const Vec3f& ILight::GetColor() const noexcept
	{
		return mColor;
	}

	float ILight::GetIntensity() const noexcept
	{
		return mIntensity;
	}

	float ILight::GetAmbient() const noexcept
	{
		return mLightComps.x;
	}

	float ILight::GetDiffuse() const noexcept
	{
		return mLightComps.y;
	}

	float ILight::GetSpecular() const noexcept
	{
		return mLightComps.z;
	}

	const Vec3f& ILight::GetLightComponents() const noexcept
	{
		return mLightComps;
	}


	void ILight::SetColor(const Vec3f& _color)
	{
		if (mColor == _color)
			return;

		mColorDirty = true;
		mColor = _color;
	}

	void ILight::SetIntensity(float _intensity)
	{
		if (Maths::Equals(mIntensity, _intensity))
			return;

		mIntensityDirty = true;
		mIntensity = _intensity;
	}

	void ILight::SetAmbient(float _ambient)
	{
		if (Maths::Equals(mLightComps.x, _ambient))
			return;

		mLightCompsDirty = true;
		mLightComps.x = _ambient;
	}

	void ILight::SetDiffuse(float _diffuse)
	{
		if (Maths::Equals(mLightComps.y, _diffuse))
			return;

		mLightCompsDirty = true;
		mLightComps.y = _diffuse;
	}

	void ILight::SetSpecular(float _specular)
	{
		if (Maths::Equals(mLightComps.z, _specular))
			return;

		mLightCompsDirty = true;
		mLightComps.z = _specular;
	}

	void ILight::SetLightComponents(const Vec3f& _lightComps)
	{
		if (mLightComps == _lightComps)
			return;

		mLightCompsDirty = true;
		mLightComps = _lightComps;
	}


	ILight& ILight::operator=(ILight&& _rhs) noexcept
	{
		mColor = Move(_rhs.mColor);

		mIntensity = _rhs.mIntensity;

		mLightComps = Move(_rhs.mLightComps);

		mColorDirty = true;
		mIntensityDirty = true;
		mLightCompsDirty = true;

		return *this;
	}

	ILight& ILight::operator=(const ILight& _rhs) noexcept
	{
		mColor = _rhs.mColor;

		mIntensity = _rhs.mIntensity;

		mLightComps = _rhs.mLightComps;

		mColorDirty = true;
		mIntensityDirty = true;
		mLightCompsDirty = true;

		return *this;
	}
}