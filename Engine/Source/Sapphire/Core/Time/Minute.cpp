// Copyright 2020 Sapphire development team.All Rights Reserved.

#include <Core/Time/Minute.hpp>

#include <Core/Time/Time.hpp>

namespace Sa
{
	Minute::Minute(float _time) noexcept : mTime{ _time }
	{
	}

	Minute::Minute(uint64 _time) noexcept : mTime{ static_cast<float>(_time) }
	{
	}


	Minute::Minute(Tick _time) noexcept : mTime{ _time.operator Minute() }
	{
	}

	Minute::Minute(MilliSecond _time) noexcept : mTime{ _time.operator Minute() }
	{
	}

	Minute::Minute(Second _time) noexcept : mTime{ _time.operator Minute() }
	{
	}

	Minute::Minute(Hour _time) noexcept : mTime{ _time.operator Minute() }
	{
	}


	Minute::operator float() const noexcept
	{
		return mTime;
	}

	Minute::operator Tick() const noexcept
	{
		return Tick(mTime * ToTicks);
	}

	Minute::operator MilliSecond() const noexcept
	{
		return MilliSecond(mTime * ToMilliSeconds);
	}

	Minute::operator Second() const noexcept
	{
		return Second(mTime * ToSeconds);
	}

	Minute::operator Hour() const noexcept
	{
		return Hour(mTime * ToHours);
	}


	Minute operator""_min(uint64 _lit) noexcept
	{
		return Minute(_lit);
	}

	Minute operator""_min(long double _lit) noexcept
	{
		return Minute(static_cast<float>(_lit));
	}
}
