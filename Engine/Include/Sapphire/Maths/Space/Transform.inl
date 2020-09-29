// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	// === TransfBase P ===
	template <typename T>
	constexpr TransfBase<T, static_cast<uint8>(TrComp::Position)>::TransfBase(const Vec3<T>& _position) noexcept :
		position{ _position }
	{
	}


	// === TransfBase R ===
	template <typename T>
	constexpr TransfBase<T, static_cast<uint8>(TrComp::Rotation)>::TransfBase(const Quat<T>& _rotation) noexcept :
		rotation{ _rotation }
	{
	}


	// === TransfBase S ===
	template <typename T>
	constexpr TransfBase<T, static_cast<uint8>(TrComp::Scale)>::TransfBase(const Vec3<T>& _scale) noexcept :
		scale{ _scale }
	{
	}




	// === TransfBase PR ===
	template <typename T>
	constexpr TransfBase<T, TrComp::Position | TrComp::Rotation>::TransfBase(const Vec3<T>& _position, const Quat<T>& _rotation) noexcept :
		position{ _position },
		rotation{ _rotation }
	{
	}



	// === TransfBase PS ===
	template <typename T>
	constexpr TransfBase<T, TrComp::Position | TrComp::Scale>::TransfBase(const Vec3<T>& _position, const Vec3<T>& _scale) noexcept :
		position{ _position },
		scale{ _scale }
	{
	}



	// === TransfBase RS ===
	template <typename T>
	constexpr TransfBase<T, TrComp::Rotation | TrComp::Scale>::TransfBase(const Quat<T>& _rotation, const Vec3<T>& _scale) noexcept :
		rotation{ _rotation },
		scale{ _scale }
	{
	}



	// === TransfBase PRS ===
	template <typename T>
	constexpr TransfBase<T, TrComp::Position | TrComp::Rotation | TrComp::Scale>::TransfBase(const Vec3<T>& _position, const Quat<T>& _rotation, const Vec3<T>& _scale) noexcept :
		position{ _position },
		rotation{ _rotation },
		scale{ _scale }
	{
	}



	// === Transf ===
	template <typename T, uint8 TrComps>
	const Transf<T, TrComps> Transf<T, TrComps>::Zero = TransfPRS(Vec3<T>::Zero, Quat<T>::Zero, Vec3<T>::Zero);

	template <typename T, uint8 TrComps>
	const Transf<T, TrComps> Transf<T, TrComps>::Identity = TransfPRS(Vec3<T>::Zero, Quat<T>::Identity, Vec3<T>::One);


	template <typename T, uint8 TrComps>
	template <typename TIn, uint8 CIn>
	constexpr Transf<T, TrComps>::Transf(const Transf<TIn, CIn>& _other) noexcept
	{
		if constexpr (TrComps & static_cast<uint8>(TrComp::Position) && CIn & static_cast<uint8>(TrComp::Position) != 0)
			position = _other.position;
		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation) && CIn & static_cast<uint8>(TrComp::Rotation) != 0)
			rotation = _other.rotation;
		if constexpr (TrComps & static_cast<uint8>(TrComp::Scale) && CIn & static_cast<uint8>(TrComp::Scale) != 0)
			scale = _other.scale;
	}


	template <typename T, uint8 TrComps>
	constexpr bool Transf<T, TrComps>::IsZero() const noexcept
	{
		if constexpr (TrComps & static_cast<uint8>(TrComp::Position | TrComp::Rotation | TrComp::Scale) != 0)
			return position.IsZero() && rotation.IsZero() && scale.IsZero();

		if constexpr (TrComps & static_cast<uint8>(TrComp::Position | TrComp::Rotation) != 0)
			return position.IsZero() && rotation.IsZero();

		if constexpr (TrComps & static_cast<uint8>(TrComp::Position | TrComp::Scale) != 0)
			return position.IsZero() && scale.IsZero();

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation | TrComp::Scale) != 0)
			return rotation.IsZero() && scale.IsZero();

		if constexpr (TrComps & static_cast<uint8>(TrComp::Position) != 0)
			return position.IsZero();

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation) != 0)
			return rotation.IsZero();

		if constexpr (TrComps & static_cast<uint8>(TrComp::Scale) != 0)
			return scale.IsZero();
	}

	template <typename T, uint8 TrComps>
	constexpr bool Transf<T, TrComps>::IsIdentity() const noexcept
	{
		if constexpr (TrComps & static_cast<uint8>(TrComp::Position | TrComp::Rotation | TrComp::Scale) != 0)
			return position.IsZero() && rotation.IsIdentity() && scale.Equals(Vec3<T>::One);

		if constexpr (TrComps & static_cast<uint8>(TrComp::Position | TrComp::Rotation) != 0)
			return position.IsZero() && rotation.IsIdentity();

		if constexpr (TrComps & static_cast<uint8>(TrComp::Position | TrComp::Scale) != 0)
			return position.IsZero() && scale.Equals(Vec3<T>::One);

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation | TrComp::Scale) != 0)
			return rotation.IsIdentity() && scale.Equals(Vec3<T>::One);

		if constexpr (TrComps & static_cast<uint8>(TrComp::Position) != 0)
			return position.IsZero();

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation) != 0)
			return rotation.IsIdentity();

		if constexpr (TrComps & static_cast<uint8>(TrComp::Scale) != 0)
			return scale.Equals(Vec3<T>::One);
	}

	template <typename T, uint8 TrComps>
	constexpr bool Transf<T, TrComps>::Equals(const Transf<T, TrComps>& _other, T _threshold) const noexcept
	{
		if constexpr (TrComps & static_cast<uint8>(TrComp::Position | TrComp::Rotation | TrComp::Scale) != 0)
		{
			return position.Equals(_other.position, _threshold) &&
				rotation.Equals(_other.rotation, _threshold) &&
				scale.Equals(_other.scale, _threshold);
		}

		if constexpr (TrComps & static_cast<uint8>(TrComp::Position | TrComp::Rotation) != 0)
		{
			return position.Equals(_other.position, _threshold) &&
				rotation.Equals(_other.rotation, _threshold);
		}

		if constexpr (TrComps & static_cast<uint8>(TrComp::Position | TrComp::Scale) != 0)
		{
			return position.Equals(_other.position, _threshold) &&
				scale.Equals(_other.scale, _threshold);
		}

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation | TrComp::Scale) != 0)
		{
			return rotation.Equals(_other.rotation, _threshold) &&
				scale.Equals(_other.scale, _threshold);
		}

		if constexpr (TrComps & static_cast<uint8>(TrComp::Position) != 0)
			return position.Equals(_other.position, _threshold);

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation) != 0)
			return rotation.Equals(_other.rotation, _threshold);

		if constexpr (TrComps & static_cast<uint8>(TrComp::Scale) != 0)
			return scale.Equals(_other.scale, _threshold);
	}

	template <typename T, uint8 TrComps>
	T* Transf<T, TrComps>::Data() noexcept
	{
		if constexpr (TrComps & static_cast<uint8>(TrComp::Position) != 0)
			return position.Data();

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation) != 0)
			return rotation.Data();

		if constexpr (TrComps & static_cast<uint8>(TrComp::Scale) != 0)
			return scale.Data();
	}

	template <typename T, uint8 TrComps>
	const T* Transf<T, TrComps>::Data() const noexcept
	{
		if constexpr (TrComps & static_cast<uint8>(TrComp::Position) != 0)
			return position.Data();

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation) != 0)
			return rotation.Data();

		if constexpr (TrComps & static_cast<uint8>(TrComp::Scale) != 0)
			return scale.Data();
	}

	template <typename T, uint8 TrComps>
	Vec3<T> Transf<T, TrComps>::RightVector() const
	{
		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation) != 0)
			return rotation.RightVector();

		return Vec3<T>::Right;
	}

	template <typename T, uint8 TrComps>
	Vec3<T> Transf<T, TrComps>::UpVector() const
	{
		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation) != 0)
			return rotation.UpVector();

		return Vec3<T>::Up;
	}

	template <typename T, uint8 TrComps>
	Vec3<T> Transf<T, TrComps>::ForwardVector() const
	{
		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation) != 0)
			return rotation.ForwardVector();

		return Vec3<T>::Forward;
	}

	template <typename T, uint8 TrComps>
	Mat4<T> Transf<T, TrComps>::Matrix() const
	{
		if constexpr (TrComps & static_cast<uint8>(TrComp::Position | TrComp::Rotation | TrComp::Scale) != 0)
			return Mat4<T>::MakeTransform(position, rotation, scale);

		if constexpr (TrComps & static_cast<uint8>(TrComp::Position | TrComp::Rotation) != 0)
			return Mat4<T>::MakeTransform(position, rotation);

		if constexpr (TrComps & static_cast<uint8>(TrComp::Position | TrComp::Scale) != 0)
			return Mat4<T>::MakeTransform(position, scale);

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation | TrComp::Scale) != 0)
			return Mat4<T>::MakeTransform(rotation, scale);

		if constexpr (TrComps & static_cast<uint8>(TrComp::Position) != 0)
			return Mat4<T>::MakeTranslation(position);

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation) != 0)
			return Mat4<T>::MakeRotation(rotation);

		if constexpr (TrComps & static_cast<uint8>(TrComp::Scale) != 0)
			return Mat4<T>::MakeScale(scale);
	}


	template <typename T, uint8 TrComps>
	Transf<T, TrComps> Transf<T, TrComps>::Lerp(const Transf<T, TrComps>& _start, const Transf<T, TrComps>& _end, float _alpha) noexcept
	{
		return LerpUnclamped(_start, _end, Maths::Clamp(_alpha, 0.0f, 1.0f));
	}

	template <typename T, uint8 TrComps>
	Transf<T, TrComps> Transf<T, TrComps>::LerpUnclamped(const Transf<T, TrComps>& _start, const Transf<T, TrComps>& _end, float _alpha) noexcept
	{
		// Always use SLerp for smooth rotation.

		if constexpr (TrComps & static_cast<uint8>(TrComp::Position | TrComp::Rotation | TrComp::Scale) != 0)
		{
			return Transf(
				Vec3.LerpUnclamped(_start.position, _end.position, _alpha),
				Quat.SLerpUnclamped(_start.rotation, _end.rotation, _alpha),
				Vec3.LerpUnclamped(_start.scale, _end.scale, _alpha));
		}

		if constexpr (TrComps & static_cast<uint8>(TrComp::Position | TrComp::Rotation) != 0)
		{
			return Transf(
				Vec3.LerpUnclamped(_start.position, _end.position, _alpha),
				Quat.SLerpUnclamped(_start.rotation, _end.rotation, _alpha));
		}

		if constexpr (TrComps & static_cast<uint8>(TrComp::Position | TrComp::Scale) != 0)
		{
			return Transf(
				Vec3.LerpUnclamped(_start.position, _end.position, _alpha),
				Vec3.LerpUnclamped(_start.scale, _end.scale, _alpha));
		}

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation | TrComp::Scale) != 0)
		{
			return Transf(
				Quat.SLerpUnclamped(_start.rotation, _end.rotation, _alpha),
				Vec3.LerpUnclamped(_start.scale, _end.scale, _alpha));
		}

		if constexpr (TrComps & static_cast<uint8>(TrComp::Position) != 0)
			return Transf(Vec3.LerpUnclamped(_start.position, _end.position, _alpha));

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation) != 0)
			return Transf(Quat.SLerpUnclamped(_start.rotation, _end.rotation, _alpha));

		if constexpr (TrComps & static_cast<uint8>(TrComp::Scale) != 0)
			return Transf(Vec3.LerpUnclamped(_start.scale, _end.scale, _alpha));
	}


	template <typename T, uint8 TrComps>
	template <typename TIn, uint8 CIn>
	Transf<T, TrComps>& Transf<T, TrComps>::operator=(const Transf<TIn, CIn>& _rhs)
	{
		if constexpr (TrComps & static_cast<uint8>(TrComp::Position) && CIn & static_cast<uint8>(TrComp::Position) != 0)
			return position = _rhs.position;

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation) && CIn & static_cast<uint8>(TrComp::Rotation) != 0)
			return rotation = _rhs.rotation;

		if constexpr (TrComps & static_cast<uint8>(TrComp::Scale) && CIn & static_cast<uint8>(TrComp::Scale) != 0)
			return scale = _rhs.scale;

		return *this;
	}

	template <typename T, uint8 TrComps>
	template <uint8 CIn>
	Transf<T, TrComps> Transf<T, TrComps>::operator*(const Transf<T, CIn>& _other) const
	{
		/**
		*	Transf(
		*		position + rotation.Rotate(_other.position),
		*		rotation * _other.rotation,
		*		scale * _other.scale);
		*/

		Transf<T, TrComps> result = *this;

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation) != 0)
		{
			if constexpr (TrComps & static_cast<uint8>(TrComp::Position) && CIn & static_cast<uint8>(TrComp::Position) != 0)
				result.position += rotation.Rotate(_other.position);

			if constexpr (CIn & static_cast<uint8>(TrComp::Rotation) != 0)
				result.rotation *= _other.rotation;
		}

		if constexpr (CIn & static_cast<uint8>(TrComp::Scale) != 0)
			result.scale *= _other.scale;

		return result;
	}

	template <typename T, uint8 TrComps>
	template <uint8 CIn>
	Transf<T, TrComps> Transf<T, TrComps>::operator/(const Transf<T, CIn>& _other) const
	{
		/**
		*	return Transf(
		*		position - rotation.Rotate(_other.position),
		*		rotation / _other.rotation,
		*		scale / _other.scale
		*	);
		*/

		Transf<T, TrComps> result = *this;

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation) != 0)
		{
			if constexpr (TrComps & static_cast<uint8>(TrComp::Position) && CIn & static_cast<uint8>(TrComp::Position) != 0)
				result.position -= rotation.Rotate(_other.position);

			if constexpr (CIn & static_cast<uint8>(TrComp::Rotation) != 0)
				result.rotation /= _other.rotation;
		}

		if constexpr (CIn & static_cast<uint8>(TrComp::Scale) != 0)
			result.scale /= _other.scale;

		return result;
	}

	template <typename T, uint8 TrComps>
	template <uint8 CIn>
	Transf<T, TrComps>& Transf<T, TrComps>::operator*=(const Transf<T, CIn>& _other)
	{
		/**
		*	Transf(
		*		position + rotation.Rotate(_other.position),
		*		rotation * _other.rotation,
		*		scale * _other.scale);
		*/

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation) != 0)
		{
			if constexpr (TrComps & static_cast<uint8>(TrComp::Position) && CIn & static_cast<uint8>(TrComp::Position) != 0)
				position += rotation.Rotate(_other.position);

			if constexpr (CIn & static_cast<uint8>(TrComp::Rotation) != 0)
				rotation *= _other.rotation;
		}

		if constexpr (CIn & static_cast<uint8>(TrComp::Scale) != 0)
			scale *= _other.scale;

		return *this;
	}

	template <typename T, uint8 TrComps>
	template <uint8 CIn>
	Transf<T, TrComps>& Transf<T, TrComps>::operator/=(const Transf<T, CIn>& _other)
	{
		/**
		*	return Transf(
		*		position - rotation.Rotate(_other.position),
		*		rotation / _other.rotation,
		*		scale / _other.scale
		*	);
		*/

		if constexpr (TrComps & static_cast<uint8>(TrComp::Rotation) != 0)
		{
			if constexpr (TrComps & static_cast<uint8>(TrComp::Position) && CIn & static_cast<uint8>(TrComp::Position) != 0)
				position -= rotation.Rotate(_other.position);

			if constexpr (CIn & static_cast<uint8>(TrComp::Rotation) != 0)
				rotation /= _other.rotation;
		}

		if constexpr (CIn & static_cast<uint8>(TrComp::Scale) != 0)
			scale /= _other.scale;

		return *this;
	}


	template <typename T, uint8 TrComps>
	constexpr bool Transf<T, TrComps>::operator==(const Transf<T, TrComps>& _rhs) noexcept
	{
		return Equal(_rhs);
	}

	template <typename T, uint8 TrComps>
	constexpr bool Transf<T, TrComps>::operator!=(const Transf<T, TrComps>& _rhs) noexcept
	{
		return !(*this == _rhs);
	}

	template <typename T, uint8 TrComps>
	template <typename TIn, uint8 CIn>
	constexpr Transf<T, TrComps>::operator Transf<TIn, CIn>() const noexcept
	{
		return Transf<TIn, CIn>(*this);
	}
}
