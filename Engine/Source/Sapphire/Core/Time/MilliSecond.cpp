// Copyright 2020 Sapphire development team.All Rights Reserved.

#include <Core/Time/MilliSecond.hpp>

#include <Core/Time/Time.hpp>

namespace Sa
{
	MilliSecond::MilliSecond(float _time) noexcept : mTime{ _time }
	{
	}

	MilliSecond::MilliSecond(uint64 _time) noexcept : mTime{ static_cast<float>(_time) }
	{
	}


	MilliSecond::MilliSecond(Tick _time) noexcept : mTime{ _time.operator MilliSecond() }
	{
	}

	MilliSecond::MilliSecond(Second _time) noexcept : mTime{ _time.operator MilliSecond() }
	{
	}

	MilliSecond::MilliSecond(Minute _time) noexcept : mTime{ _time.operator MilliSecond() }
	{
	}

	MilliSecond::MilliSecond(Hour _time) noexcept : mTime{ _time.operator MilliSecond() }
	{
	}


	MilliSecond::operator float() const noexcept
	{
		return mTime;
	}

	MilliSecond::operator Tick() const noexcept
	{
		return Tick(mTime * ToTicks);
	}

	MilliSecond::operator Second() const noexcept
	{
		return Second(mTime  * ToSeconds);
	}

	MilliSecond::operator Minute() const noexcept
	{
		return Minute(mTime *  ToMinutes);
	}

	MilliSecond::operator Hour() const noexcept
	{
		return Hour(mTime  * ToHours);
	}


	MilliSecond operator""_ms(uint64 _lit) noexcept
	{
		return MilliSecond(_lit);
	}

	MilliSecond operator""_ms(long double _lit) noexcept
	{
		return MilliSecond(static_cast<float>(_lit));
	}
}
