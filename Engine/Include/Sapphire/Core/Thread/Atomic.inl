// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename T>
	Atomic<T>::Atomic(T _value) noexcept : mHandle{ _value }
	{
	}

	template <typename T>
	T Atomic<T>::Get() const noexcept
	{
		return mHandle.load();
	}

	template <typename T>
	void Atomic<T>::Set(T _value) noexcept
	{
		mHandle.store(_value);
	}
	
	template <typename T>
	T Atomic<T>::Exchange(T _value)
	{
		return mHandle.exchange(_value);
	}

	template <typename T>
	bool Atomic<T>::IsLockFree() noexcept
	{
		return std::atomic<T>().is_lock_free();
	}

	
	template <typename T>
	Atomic<T>& Atomic<T>::operator=(T _rhs) noexcept
	{
		mHandle = _rhs;

		return *this;
	}

	template <typename T>
	T Atomic<T>::operator++() noexcept
	{
		return ++mHandle;
	}
	
	template <typename T>
	T Atomic<T>::operator++(int32) noexcept
	{
		return mHandle++;
	}
	
	template <typename T>
	T Atomic<T>::operator--() noexcept
	{
		return --mHandle;
	}
	
	template <typename T>
	T Atomic<T>::operator--(int32) noexcept
	{
		return mHandle--;
	}

	
	template <typename T>
	T Atomic<T>::operator+=(T _rhs) noexcept
	{
		return mHandle += _rhs;
	}
	
	template <typename T>
	T Atomic<T>::operator-=(T _rhs) noexcept
	{
		return mHandle -= _rhs;
	}
	
	template <typename T>
	T Atomic<T>::operator&=(T _rhs) noexcept
	{
		return mHandle &= _rhs;
	}
	
	template <typename T>
	T Atomic<T>::operator|=(T _rhs) noexcept
	{
		return mHandle |= _rhs;
	}
	
	template <typename T>
	T Atomic<T>::operator^=(T _rhs) noexcept
	{
		return mHandle ^= _rhs;
	}

	
	template <typename T>
	Atomic<T>::operator T() const noexcept
	{
		return mHandle;
	}
}
