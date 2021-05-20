// Copyright 2020 Sapphire development team.All Rights Reserved.

#include <Core/Time/Tick.hpp>

#include <Core/Time/Time.hpp>

namespace Sa
{
	Tick::Tick(float _time) noexcept : mTime{ _time }
	{
	}

	Tick::Tick(uint64 _time) noexcept : mTime{ static_cast<float>(_time) }
	{
	}


	Tick::Tick(MilliSecond _time) noexcept : mTime{ _time.operator Sa::Tick() }
	{
	}

	Tick::Tick(Second _time) noexcept : mTime{ _time.operator Sa::Tick() }
	{
	}

	Tick::Tick(Minute _time) noexcept : mTime{ _time.operator Sa::Tick() }
	{
	}

	Tick::Tick(Hour _time) noexcept : mTime{ _time.operator Sa::Tick() }
	{
	}


	Tick::operator float() const noexcept
	{
		return mTime;
	}

	Tick::operator MilliSecond() const noexcept
	{
		return MilliSecond(mTime * ToMilliSeconds);
	}

	Tick::operator Second() const noexcept
	{
		return Second(mTime * ToSeconds);
	}

	Tick::operator Minute() const noexcept
	{
		return Second(mTime * ToMinutes);
	}

	Tick::operator Hour() const noexcept
	{
		return Second(mTime * ToHours);
	}


	Tick operator""_tk(uint64 _lit) noexcept
	{
		return Tick(_lit);
	}

	Tick operator""_tk(long double _lit) noexcept
	{
		return Tick(static_cast<float>(_lit));
	}
}
