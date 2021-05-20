// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Thread/Mutex.hpp>

namespace Sa
{
	void Mutex::Lock()
	{
		mHandle.lock();
	}

	bool Mutex::TryLock()
	{
		return mHandle.try_lock();
	}

	void Mutex::Unlock()
	{
		mHandle.unlock();
	}


	RAII<Mutex>::RAII(Mutex& _mutex) : mHandle{ _mutex }
	{
		mHandle.Lock();
	}

	RAII<Mutex>::~RAII()
	{
		mHandle.Unlock();
	}
}
