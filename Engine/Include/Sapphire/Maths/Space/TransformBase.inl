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
		constexpr TransfBase<T, TrComp::PR>::TransfBase(const Vec3<T>& _position, const Quat<T>& _rotation) noexcept :
			position{ _position },
			rotation{ _rotation }
		{
		}



		// === TransfBase PS ===
		template <typename T>
		constexpr TransfBase<T, TrComp::PS>::TransfBase(const Vec3<T>& _position, const Vec3<T>& _scale) noexcept :
			position{ _position },
			scale{ _scale }
		{
		}



		// === TransfBase RS ===
		template <typename T>
		constexpr TransfBase<T, TrComp::RS>::TransfBase(const Quat<T>& _rotation, const Vec3<T>& _scale) noexcept :
			rotation{ _rotation },
			scale{ _scale }
		{
		}



		// === TransfBase PRS ===
		template <typename T>
		constexpr TransfBase<T, TrComp::PRS>::TransfBase(const Vec3<T>& _position, const Quat<T>& _rotation, const Vec3<T>& _scale) noexcept :
			position{ _position },
			rotation{ _rotation },
			scale{ _scale }
		{
		}
	}
}
