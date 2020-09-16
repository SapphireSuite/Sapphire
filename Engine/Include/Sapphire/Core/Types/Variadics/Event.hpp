// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_CORE_EVENT_GUARD
#define SAPPHIRE_CORE_EVENT_GUARD

#include <vector>
#include <functional>

namespace Sa
{
	template <typename T>
	class Event;

	template <typename R, typename... Args>
	class Event<R(Args...)>
	{
		std::vector<std::function<R(Args...)>> functions;

	public:
		void Add(const std::function<R(Args...)>& _func)
		{
			functions.push_back(_func);
		}

		void Clear()
		{
			functions.clear();
		}

		void Call(const Args&... _args)
		{
			for (auto it = functions.begin(); it != functions.end(); ++it)
				(*it)(_args...);
		}

		void operator()(const Args&... _args)
		{
			Call(_args...);
		}
	};
}

#endif // GUARD
