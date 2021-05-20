// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename Function, typename... Args>
	Thread::Thread(Function&& _func, Args&&... _args) : mHandle{ Move(_func), Forward<Args>(_args)... }
	{
	}
}
