// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_EVENT_GUARD
#define SAPPHIRE_CORE_EVENT_GUARD

#include <vector>

#include <Collections/Debug>

namespace Sa
{
	template <typename R, typename... Args>
	class Event;

	template <typename R, typename... Args>
	class Event<R(Args...)>
	{
		struct FuncHandle
		{
			R(* func)(Args...) = nullptr;
		};

		std::vector<FuncHandle> mFunctions;


		template <typename C>
		struct FuncMemberData
		{
			C* caller = nullptr;
			R(C::* func)(Args...) = nullptr;
		};

		struct FuncMemberHandle
		{
			void* data;
			R(*func)(void*, Args...);
		};

		std::vector<FuncMemberHandle> mMemberFunctions;

	public:
		Event() = default;

		Event(Event&&) = delete;
		Event(const Event&) = delete;

		~Event();


		bool IsEmpty() const noexcept;

		void Clear();


		void Add(R(*_func)(Args...));

		template <typename C>
		void Add(C* _caller, R(C::* _func)(Args...));


		void Remove(R(* _func)(Args...));

		template <typename C>
		void Remove(C* _caller, R(C::* _func)(Args...));


		void RRemove(R(* _func)(Args...));

		template <typename C>
		void RRemove(C* _caller, R(C::* _func)(Args...));


		void Execute(Args... _args);


		Event& operator=(Event&&) = delete;
		Event& operator=(const Event&) = delete;

		void operator+=(R(*_func)(Args...));
		void operator-=(R(*_func)(Args...));

		void operator()(Args... _args);
	};
}

#include <Core/Types/Variadics/Event.inl>

#endif // GUARD
