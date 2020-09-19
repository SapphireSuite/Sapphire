// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename T>
	const Transf<T> Transf<T>::Zero(Vec3<T>::Zero, Quat<T>::Zero, Vec3<T>::Zero);

	template <typename T>
	const Transf<T> Transf<T>::Identity(Vec3<T>::Zero, Quat<T>::Identity, Vec3<T>::One);


	template <typename T>
	Transf<T>::Transf(const Vec3<T>& _position, const Quat<T>& _rotation, const Vec3<T>& _scale) noexcept :
		position{ _position },
		rotation{ _rotation },
		scale{ _scale }
	{
	}

	template <typename T>
	template <typename TIn>
	constexpr Transf<T>::Transf(const Transf<TIn>& _other) noexcept :
		position{ _other.position },
		rotation{ _other.rotation },
		scale{ _other.scale }

	{
	}


	template <typename T>
	constexpr bool Transf<T>::IsZero() const noexcept
	{
		return position.IsZero() && rotation.IsZero() && scale.IsZero();
	}

	template <typename T>
	constexpr bool Transf<T>::IsIdentity() const noexcept
	{
		return position.IsZero() && rotation.IsIdentity() && scale.Equals(Vec3<T>::One);
	}

	template <typename T>
	constexpr bool Transf<T>::Equals(const Transf<T>& _other, T _threshold) const noexcept
	{
		return position.Equals(_other.position, _threshold) &&
			rotation.Equals(_other.rotation, _threshold) &&
			scale.Equals(_other.scale, _threshold);
	}

	template <typename T>
	T* Transf<T>::Data() noexcept
	{
		return position.Data();
	}

	template <typename T>
	const T* Transf<T>::Data() const noexcept
	{
		return position.Data();
	}


	template <typename T>
	Vec3<T> Transf<T>::RightVector() const
	{
		return rotation.RightVector();
	}

	template <typename T>
	Vec3<T> Transf<T>::UpVector() const
	{
		return rotation.UpVector();
	}

	template <typename T>
	Vec3<T> Transf<T>::ForwardVector() const
	{
		return rotation.ForwardVector();
	}

	template <typename T>
	Mat4<T> Transf<T>::Matrix() const
	{
		return Mat4<T>::MakeTransform(position, rotation, scale);
	}


	template <typename T>
	Transf<T> Transf<T>::Lerp(const Transf<T>& _start, const Transf<T>& _end, float _alpha) noexcept
	{
		return LerpUnclamped(_start, _end, Maths::Clamp(_alpha, 0.0f, 1.0f));
	}

	template <typename T>
	Transf<T> Transf<T>::LerpUnclamped(const Transf<T>& _start, const Transf<T>& _end, float _alpha) noexcept
	{
		return Transf(
			Vec3.LerpUnclamped(_start.position, _end.position, _alpha),
			Quat.SLerpUnclamped(_start.rotation, _end.rotation, _alpha), // Always use SLerp for smooth rotation.
			Vec3.LerpUnclamped(_start.scale, _end.scale, _alpha),
			);
	}


	template <typename T>
	Transf<T> Transf<T>::operator*(const Transf<T>& _other) const
	{
		return Transf(
			position + rotation.Rotate(_other.position),
			rotation * _other.rotation,
			scale * _other.scale
		);
	}

	template <typename T>
	Transf<T> Transf<T>::operator/(const Transf<T>& _other) const
	{
		return Transf(
			position - rotation.Rotate(_other.position),
			rotation / _other.rotation,
			scale / _other.scale
		);
	}

	template <typename T>
	Transf<T>& Transf<T>::operator*=(const Transf<T>& _other)
	{
		position += rotation.Rotate(_other.position);
		rotation *= _other.rotation;
		scale *= _other.scale;

		return *this;
	}

	template <typename T>
	Transf<T>& Transf<T>::operator/=(const Transf<T>& _other)
	{
		position -= rotation.Rotate(_other.position);
		rotation /= _other.rotation;
		scale /= _other.scale;
	}


	template <typename T>
	constexpr bool Transf<T>::operator==(const Transf<T>& _rhs) noexcept
	{
		return Equal(_rhs);
	}

	template <typename T>
	constexpr bool Transf<T>::operator!=(const Transf<T>& _rhs) noexcept
	{
		return !(*this == _rhs);
	}


	template <typename T>
	template <typename TIn>
	constexpr Transf<T>::operator Transf<TIn>() const noexcept
	{
		return Transf<TIn>(*this);
	}
}
