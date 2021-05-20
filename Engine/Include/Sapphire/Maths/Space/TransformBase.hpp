// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_MATHS_TRANSFORM_BASE_GUARD
#define SAPPHIRE_MATHS_TRANSFORM_BASE_GUARD

#include <Maths/Space/TrComps.hpp>
#include <Maths/Space/Vector2.hpp>
#include <Maths/Space/Vector3.hpp>

namespace Sa
{
	namespace Internal
	{
		/**
		*	\brief \e Transform base Sapphire's class.
		*
		*	\tparam T	Type of the Transform.
		*
		*	\tparam Components	Components of the transform, defined by TrComp.
		*/
		template <typename T, TrComp TrComps>
		struct TransfBase;

		template <typename T>
		struct TransfBase<T, TrComp::None>;


		// === Transf P ===
		template <typename T>
		struct TransfBase <T, TrComp::Position>
		{
			/// Position component of the transform.
			Vec3<T> position;

			/**
			*	\brief \e Default constructor.
			*/
			TransfBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			TransfBase(TransfBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			TransfBase(const TransfBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _position	Position value.
			*/
			constexpr TransfBase(const Vec3<T>& _position) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			TransfBase& operator=(TransfBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			TransfBase& operator=(const TransfBase&) = default;
		};


		// === Transf R ===
		template <typename T>
		struct TransfBase <T, TrComp::Rotation>
		{
			/// Rotation component of the transform.
			Quat<T> rotation = Quat<T>::Identity;

			/**
			*	\brief \e Default constructor.
			*/
			TransfBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			TransfBase(TransfBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			TransfBase(const TransfBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _rotation	Rotation value.
			*/
			constexpr TransfBase(const Quat<T>& _rotation) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			TransfBase& operator=(TransfBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			TransfBase& operator=(const TransfBase&) = default;
		};


		// === Transf S ===
		template <typename T>
		struct TransfBase <T, TrComp::Scale>
		{
			/// Scale component of the transform.
			Vec3<T> scale = Vec3<T>::One;

			/**
			*	\brief \e Default constructor.
			*/
			TransfBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			TransfBase(TransfBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			TransfBase(const TransfBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _scale		Scale value.
			*/
			constexpr TransfBase(const Vec3<T>& _scale) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			TransfBase& operator=(TransfBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			TransfBase& operator=(const TransfBase&) = default;
		};


		// === Transf PR ===
		template <typename T>
		struct TransfBase <T, TrComp::PR>
		{
			/// Position component of the transform.
			Vec3<T> position;

			/// Rotation component of the transform.
			Quat<T> rotation = Quat<T>::Identity;

			/**
			*	\brief \e Default constructor.
			*/
			TransfBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			TransfBase(TransfBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			TransfBase(const TransfBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _position	Position value.
			*	\param[in] _rotation	Rotation value.
			*/
			constexpr TransfBase(const Vec3<T>& _position, const Quat<T>& _rotation = Quat<T>::Identity) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			TransfBase& operator=(TransfBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			TransfBase& operator=(const TransfBase&) = default;
		};


		// === Transf PS ===
		template <typename T>
		struct TransfBase <T, TrComp::PS>
		{
			/// Position component of the transform.
			Vec3<T> position;

			/// Scale component of the transform.
			Vec3<T> scale = Vec3<T>::One;

			/**
			*	\brief \e Default constructor.
			*/
			TransfBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			TransfBase(TransfBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			TransfBase(const TransfBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _position	Position value.
			*	\param[in] _scale		Scale value.
			*/
			constexpr TransfBase(const Vec3<T>& _position, const Vec3<T>& _scale = Vec3<T>::One) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			TransfBase& operator=(TransfBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			TransfBase& operator=(const TransfBase&) = default;
		};


		// === Transf RS ===
		template <typename T>
		struct TransfBase <T, TrComp::RS>
		{
			/// Rotation component of the transform.
			Quat<T> rotation = Quat<T>::Identity;

			/// Scale component of the transform.
			Vec3<T> scale = Vec3<T>::One;

			/**
			*	\brief \e Default constructor.
			*/
			TransfBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			TransfBase(TransfBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			TransfBase(const TransfBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _rotation	Rotation value.
			*	\param[in] _scale		Scale value.
			*/
			constexpr TransfBase(const Quat<T>& _rotation, const Vec3<T>& _scale = Vec3<T>::One) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			TransfBase& operator=(TransfBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			TransfBase& operator=(const TransfBase&) = default;
		};


		// === Transf PRS ===
		template <typename T>
		struct TransfBase <T, TrComp::PRS>
		{
			/// Position component of the transform.
			Vec3<T> position;

			/// Rotation component of the transform.
			Quat<T> rotation = Quat<T>::Identity;

			/// Scale component of the transform.
			Vec3<T> scale = Vec3<T>::One;

			/**
			*	\brief \e Default constructor.
			*/
			TransfBase() = default;

			/**
			*	\brief \e Default move constructor.
			*/
			TransfBase(TransfBase&&) = default;

			/**
			*	\brief \e Default copy constructor.
			*/
			TransfBase(const TransfBase&) = default;

			/**
			*	\brief \e Value constructor.
			*
			*	\param[in] _position	Position value.
			*	\param[in] _rotation	Rotation value.
			*	\param[in] _scale		Scale value.
			*/
			constexpr TransfBase(const Vec3<T>& _position, const Quat<T>& _rotation = Quat<T>::Identity, const Vec3<T>& _scale = Vec3<T>::One) noexcept;


			/**
			*	\brief \e Default move assignment.
			*/
			TransfBase& operator=(TransfBase&&) = default;

			/**
			*	\brief \e Default copy assignment.
			*/
			TransfBase& operator=(const TransfBase&) = default;
		};
	}
}

#include <Maths/Space/TransformBase.inl>

#endif // GUARD
