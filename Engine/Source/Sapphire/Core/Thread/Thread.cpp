// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Thread/Thread.hpp>

namespace Sa
{
	Thread::~Thread() noexcept
	{
		Join();
	}


	uint32 Thread::GetID() const noexcept
	{
		std::thread::id id = mHandle.get_id();
		return reinterpret_cast<uint32&>(id);
	}

	bool Thread::IsRunning() const noexcept
	{
		return mHandle.joinable();
	}


	void Thread::Join() noexcept
	{
		if (mHandle.joinable())
			mHandle.join();
	}

	void Thread::Detatch() noexcept
	{
		mHandle.detach();
	}


	uint32 Thread::HardwareConcurrency()
	{
		return std::thread::hardware_concurrency();
	}

	void Thread::Yield()
	{
		std::this_thread::yield();
	}
}
