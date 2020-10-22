// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	namespace Internal
	{
		// === TransfBase P ===
		template <typename T>
		constexpr TransfBase<T, TrComp::Position>::TransfBase(const Vec3<T>& _position) noexcept :
			position{ _position }
		{
		}


		// === TransfBase R ===
		template <typename T>
		constexpr TransfBase<T, TrComp::Rotation>::TransfBase(const Quat<T>& _rotation) noexcept :
			rotation{ _rotation }
		{
		}


		// === TransfBase S ===
		template <typename T>
		constexpr TransfBase<T, TrComp::Scale>::TransfBase(const Vec3<T>& _scale) noexcept :
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
	}



	// === Transf ===
	template <typename T, TrComp TrComps>
	const Transf<T, TrComps> Transf<T, TrComps>::Zero = TransfPRS(Vec3<T>::Zero, Quat<T>::Zero, Vec3<T>::Zero);

	template <typename T, TrComp TrComps>
	const Transf<T, TrComps> Transf<T, TrComps>::Identity = TransfPRS(Vec3<T>::Zero, Quat<T>::Identity, Vec3<T>::One);


	template <typename T, TrComp TrComps>
	template <typename TIn, TrComp CIn>
	constexpr Transf<T, TrComps>::Transf(const Transf<TIn, CIn>& _other) noexcept
	{
		if constexpr ((TrComps & TrComp::Position) != TrComp::None && (CIn & TrComp::Position) != TrComp::None)
			Base::position = _other.position;

		if constexpr ((TrComps & TrComp::Rotation) != TrComp::None && (CIn & TrComp::Rotation) != TrComp::None)
			Base::rotation = _other.rotation;

		if constexpr ((TrComps & TrComp::Scale) != TrComp::None && (CIn & TrComp::Scale) != TrComp::None)
			Base::scale = _other.scale;
	}


	template <typename T, TrComp TrComps>
	constexpr bool Transf<T, TrComps>::IsZero() const noexcept
	{
		if constexpr ((TrComps & TrComp::Position | TrComp::Rotation | TrComp::Scale) != TrComp::None)
			return Base::position.IsZero() && Base::rotation.IsZero() && Base::scale.IsZero();

		else if constexpr ((TrComps & TrComp::Position | TrComp::Rotation) != TrComp::None)
			return Base::position.IsZero() && Base::rotation.IsZero();

		else if constexpr ((TrComps & TrComp::Position | TrComp::Scale) != TrComp::None)
			return Base::position.IsZero() && Base::scale.IsZero();

		else if constexpr ((TrComps & TrComp::Rotation | TrComp::Scale) != TrComp::None)
			return Base::rotation.IsZero() && Base::scale.IsZero();

		else if constexpr ((TrComps & TrComp::Position) != TrComp::None)
			return Base::position.IsZero();

		else if constexpr ((TrComps & TrComp::Rotation) != TrComp::None)
			return Base::rotation.IsZero();

		else if constexpr ((TrComps & TrComp::Scale) != TrComp::None)
			return Base::scale.IsZero();

		else
			return false;
	}

	template <typename T, TrComp TrComps>
	constexpr bool Transf<T, TrComps>::IsIdentity() const noexcept
	{
		if constexpr ((TrComps & TrComp::Position | TrComp::Rotation | TrComp::Scale) != TrComp::None)
			return Base::position.IsZero() && Base::rotation.IsIdentity() && Base::scale.Equals(Vec3<T>::One);

		else if constexpr ((TrComps & TrComp::Position | TrComp::Rotation) != TrComp::None)
			return Base::position.IsZero() && Base::rotation.IsIdentity();

		else if constexpr ((TrComps & TrComp::Position | TrComp::Scale) != TrComp::None)
			return Base::position.IsZero() && Base::scale.Equals(Vec3<T>::One);

		else if constexpr ((TrComps & TrComp::Rotation | TrComp::Scale) != TrComp::None)
			return Base::rotation.IsIdentity() && Base::scale.Equals(Vec3<T>::One);

		else if constexpr ((TrComps & TrComp::Position) != TrComp::None)
			return Base::position.IsZero();

		else if constexpr ((TrComps & TrComp::Rotation) != TrComp::None)
			return Base::rotation.IsIdentity();

		else if constexpr ((TrComps & TrComp::Scale) != TrComp::None)
			return Base::scale.Equals(Vec3<T>::One);
		
		else
			return false;
	}

	template <typename T, TrComp TrComps>
	constexpr bool Transf<T, TrComps>::Equals(const Transf<T, TrComps>& _other, T _threshold) const noexcept
	{
		if constexpr ((TrComps & TrComp::Position | TrComp::Rotation | TrComp::Scale) != TrComp::None)
		{
			return Base::position.Equals(_other.position, _threshold) &&
				Base::rotation.Equals(_other.rotation, _threshold) &&
				Base::scale.Equals(_other.scale, _threshold);
		}

		else if constexpr ((TrComps & TrComp::Position | TrComp::Rotation) != TrComp::None)
		{
			return Base::position.Equals(_other.position, _threshold) &&
				Base::rotation.Equals(_other.rotation, _threshold);
		}

		else if constexpr ((TrComps & TrComp::Position | TrComp::Scale) != TrComp::None)
		{
			return Base::position.Equals(_other.position, _threshold) &&
				Base::scale.Equals(_other.scale, _threshold);
		}

		else if constexpr ((TrComps & TrComp::Rotation | TrComp::Scale) != TrComp::None)
		{
			return Base::rotation.Equals(_other.rotation, _threshold) &&
				Base::scale.Equals(_other.scale, _threshold);
		}

		else if constexpr ((TrComps & TrComp::Position) != TrComp::None)
			return Base::position.Equals(_other.position, _threshold);

		else if constexpr ((TrComps & TrComp::Rotation) != TrComp::None)
			return Base::rotation.Equals(_other.rotation, _threshold);

		else if constexpr ((TrComps & TrComp::Scale) != TrComp::None)
			return Base::scale.Equals(_other.scale, _threshold);
		
		else
			return false;
	}

	template <typename T, TrComp TrComps>
	T* Transf<T, TrComps>::Data() noexcept
	{
		if constexpr ((TrComps & TrComp::Position) != TrComp::None)
			return Base::position.Data();

		else if constexpr ((TrComps & TrComp::Rotation) != TrComp::None)
			return Base::rotation.Data();

		else if constexpr ((TrComps & TrComp::Scale) != TrComp::None)
			return Base::scale.Data();

		else
			return nullptr;
	}

	template <typename T, TrComp TrComps>
	const T* Transf<T, TrComps>::Data() const noexcept
	{
		if constexpr ((TrComps & TrComp::Position) != TrComp::None)
			return Base::position.Data();

		else if constexpr ((TrComps & TrComp::Rotation) != TrComp::None)
			return Base::rotation.Data();

		else if constexpr ((TrComps & TrComp::Scale) != TrComp::None)
			return Base::scale.Data();

		else
			return nullptr;
	}

	template <typename T, TrComp TrComps>
	Vec3<T> Transf<T, TrComps>::RightVector() const
	{
		if constexpr ((TrComps & TrComp::Rotation) != TrComp::None)
			return Base::rotation.RightVector();
		else
			return Vec3<T>::Right;
	}

	template <typename T, TrComp TrComps>
	Vec3<T> Transf<T, TrComps>::UpVector() const
	{
		if constexpr ((TrComps & TrComp::Rotation) != TrComp::None)
			return Base::rotation.UpVector();
		else
			return Vec3<T>::Up;
	}

	template <typename T, TrComp TrComps>
	Vec3<T> Transf<T, TrComps>::ForwardVector() const
	{
		if constexpr ((TrComps & TrComp::Rotation) != TrComp::None)
			return Base::rotation.ForwardVector();
		else
			return Vec3<T>::Forward;
	}

	template <typename T, TrComp TrComps>
	Mat4<T> Transf<T, TrComps>::Matrix() const
	{
		if constexpr ((TrComps & TrComp::Position | TrComp::Rotation | TrComp::Scale) != TrComp::None)
			return Mat4<T>::MakeTransform(Base::position, Base::rotation, Base::scale);

		else if constexpr ((TrComps & TrComp::Position | TrComp::Rotation) != TrComp::None)
			return Mat4<T>::MakeTransform(Base::position, Base::rotation);

		else if constexpr ((TrComps & TrComp::Position | TrComp::Scale) != TrComp::None)
			return Mat4<T>::MakeTransform(Base::position, Base::scale);

		else if constexpr ((TrComps & TrComp::Rotation | TrComp::Scale) != TrComp::None)
			return Mat4<T>::MakeTransform(Base::rotation, Base::scale);

		else if constexpr ((TrComps & TrComp::Position) != TrComp::None)
			return Mat4<T>::MakeTranslation(Base::position);

		else if constexpr ((TrComps & TrComp::Rotation) != TrComp::None)
			return Mat4<T>::MakeRotation(Base::rotation);

		else if constexpr ((TrComps & TrComp::Scale) != TrComp::None)
			return Mat4<T>::MakeScale(Base::scale);

		else
			return Mat4<T>::Identity;
	}


	template <typename T, TrComp TrComps>
	Transf<T, TrComps> Transf<T, TrComps>::Lerp(const Transf<T, TrComps>& _start, const Transf<T, TrComps>& _end, float _alpha) noexcept
	{
		return LerpUnclamped(_start, _end, Maths::Clamp(_alpha, 0.0f, 1.0f));
	}

	template <typename T, TrComp TrComps>
	Transf<T, TrComps> Transf<T, TrComps>::LerpUnclamped(const Transf<T, TrComps>& _start, const Transf<T, TrComps>& _end, float _alpha) noexcept
	{
		// Always use SLerp for smooth rotation.

		if constexpr ((TrComps & TrComp::Position | TrComp::Rotation | TrComp::Scale) != TrComp::None)
		{
			return Transf(
				Vec3.LerpUnclamped(_start.position, _end.position, _alpha),
				Quat.SLerpUnclamped(_start.rotation, _end.rotation, _alpha),
				Vec3.LerpUnclamped(_start.scale, _end.scale, _alpha));
		}

		else if constexpr ((TrComps & TrComp::Position | TrComp::Rotation) != TrComp::None)
		{
			return Transf(
				Vec3.LerpUnclamped(_start.position, _end.position, _alpha),
				Quat.SLerpUnclamped(_start.rotation, _end.rotation, _alpha));
		}

		else if constexpr ((TrComps & TrComp::Position | TrComp::Scale) != TrComp::None)
		{
			return Transf(
				Vec3.LerpUnclamped(_start.position, _end.position, _alpha),
				Vec3.LerpUnclamped(_start.scale, _end.scale, _alpha));
		}

		else if constexpr ((TrComps & TrComp::Rotation | TrComp::Scale) != TrComp::None)
		{
			return Transf(
				Quat.SLerpUnclamped(_start.rotation, _end.rotation, _alpha),
				Vec3.LerpUnclamped(_start.scale, _end.scale, _alpha));
		}

		else if constexpr ((TrComps & TrComp::Position) != TrComp::None)
			return Transf(Vec3.LerpUnclamped(_start.position, _end.position, _alpha));

		else if constexpr ((TrComps & TrComp::Rotation) != TrComp::None)
			return Transf(Quat.SLerpUnclamped(_start.rotation, _end.rotation, _alpha));

		else if constexpr ((TrComps & TrComp::Scale) != TrComp::None)
			return Transf(Vec3.LerpUnclamped(_start.scale, _end.scale, _alpha));

		else
			Transf::Identity;
	}


	template <typename T, TrComp TrComps>
	template <typename TIn, TrComp CIn>
	Transf<T, TrComps>& Transf<T, TrComps>::operator=(const Transf<TIn, CIn>& _rhs)
	{
		if constexpr ((TrComps & TrComp::Position) != TrComp::None && (CIn & TrComp::Position) != TrComp::None)
			return position = _rhs.position;

		if constexpr ((TrComps & TrComp::Rotation) != TrComp::None && (CIn & TrComp::Rotation) != TrComp::None)
			return rotation = _rhs.rotation;

		if constexpr ((TrComps & TrComp::Scale) != TrComp::None && (CIn & TrComp::Scale) != TrComp::None)
			return scale = _rhs.scale;

		return *this;
	}

	template <typename T, TrComp TrComps>
	template <TrComp CIn>
	Transf<T, TrComps> Transf<T, TrComps>::operator*(const Transf<T, CIn>& _other) const
	{
		/**
		*	Transf(
		*		position + rotation.Rotate(_other.position),
		*		rotation * _other.rotation,
		*		scale * _other.scale);
		*/

		Transf<T, TrComps> result = *this;

		if constexpr ((TrComps & TrComp::Rotation) != TrComp::None)
		{
			if constexpr ((TrComps & TrComp::Position) != TrComp::None && (CIn & TrComp::Position) != TrComp::None)
				result.position += rotation.Rotate(_other.position);

			if constexpr ((CIn & TrComp::Rotation) != TrComp::None)
				result.rotation *= _other.rotation;
		}

		if constexpr ((CIn & TrComp::Scale) != TrComp::None)
			result.scale *= _other.scale;

		return result;
	}

	template <typename T, TrComp TrComps>
	template <TrComp CIn>
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

		if constexpr ((TrComps & TrComp::Rotation) != TrComp::None)
		{
			if constexpr ((TrComps & TrComp::Position) != TrComp::None && (CIn & TrComp::Position) != TrComp::None)
				result.position -= rotation.Rotate(_other.position);

			if constexpr ((CIn & TrComp::Rotation) != TrComp::None)
				result.rotation /= _other.rotation;
		}

		if constexpr ((CIn & TrComp::Scale) != TrComp::None)
			result.scale /= _other.scale;

		return result;
	}

	template <typename T, TrComp TrComps>
	template <TrComp CIn>
	Transf<T, TrComps>& Transf<T, TrComps>::operator*=(const Transf<T, CIn>& _other)
	{
		/**
		*	Transf(
		*		position + rotation.Rotate(_other.position),
		*		rotation * _other.rotation,
		*		scale * _other.scale);
		*/

		if constexpr ((TrComps & TrComp::Rotation) != TrComp::None)
		{
			if constexpr ((TrComps & TrComp::Position) != TrComp::None && (CIn & TrComp::Position) != TrComp::None)
				position += rotation.Rotate(_other.position);

			if constexpr ((CIn & TrComp::Rotation) != TrComp::None)
				rotation *= _other.rotation;
		}

		if constexpr ((CIn & TrComp::Scale) != TrComp::None)
			scale *= _other.scale;

		return *this;
	}

	template <typename T, TrComp TrComps>
	template <TrComp CIn>
	Transf<T, TrComps>& Transf<T, TrComps>::operator/=(const Transf<T, CIn>& _other)
	{
		/**
		*	return Transf(
		*		position - rotation.Rotate(_other.position),
		*		rotation / _other.rotation,
		*		scale / _other.scale
		*	);
		*/

		if constexpr ((TrComps & TrComp::Rotation) != TrComp::None)
		{
			if constexpr ((TrComps & TrComp::Position) != TrComp::None && (CIn & TrComp::Position) != TrComp::None)
				position -= rotation.Rotate(_other.position);

			if constexpr ((CIn & TrComp::Rotation) != TrComp::None)
				rotation /= _other.rotation;
		}

		if constexpr ((CIn & TrComp::Scale) != TrComp::None)
			scale /= _other.scale;

		return *this;
	}


	template <typename T, TrComp TrComps>
	constexpr bool Transf<T, TrComps>::operator==(const Transf<T, TrComps>& _rhs) noexcept
	{
		return Equals(_rhs);
	}

	template <typename T, TrComp TrComps>
	constexpr bool Transf<T, TrComps>::operator!=(const Transf<T, TrComps>& _rhs) noexcept
	{
		return !(*this == _rhs);
	}

	template <typename T, TrComp TrComps>
	template <typename TIn, TrComp CIn>
	constexpr Transf<T, TrComps>::operator Transf<TIn, CIn>() const noexcept
	{
		return Transf<TIn, CIn>(*this);
	}
}
