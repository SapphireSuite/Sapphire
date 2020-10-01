// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	namespace Internal
	{
		template <typename FuncHandle, typename FuncMemberHandle, typename R, typename... Args>
		EventBase<FuncHandle, FuncMemberHandle, R, Args...>::~EventBase()
		{
			Clear();
		}


		template <typename FuncHandle, typename FuncMemberHandle, typename R, typename... Args>
		bool EventBase<FuncHandle, FuncMemberHandle, R, Args...>::IsEmpty() const noexcept
		{
			return mFunctions.size() == 0 && mMemberFunctions.size() == 0;
		}

		template <typename FuncHandle, typename FuncMemberHandle, typename R, typename... Args>
		void EventBase<FuncHandle, FuncMemberHandle, R, Args...>::Clear()
		{
			mFunctions.clear();

			for (auto it = mMemberFunctions.begin(); it != mMemberFunctions.end(); ++it)
			{
				SA_ASSERT(it->data, Nullptr, Tools, L"Internal function call: data nullptr!");

				delete it->data;
			}

			mMemberFunctions.clear();
		}


		template <typename FuncHandle, typename FuncMemberHandle, typename R, typename... Args>
		void EventBase<FuncHandle, FuncMemberHandle, R, Args...>::Add(R(*_func)(Args...))
		{
			SA_ASSERT(_func, Nullptr, Tools, L"Add nullptr function!");

			mFunctions.push_back(FuncHandle{ _func });
		}

		template <typename FuncHandle, typename FuncMemberHandle, typename R, typename... Args>
		template <typename C>
		void EventBase<FuncHandle, FuncMemberHandle, R, Args...>::Add(C* _caller, R(C::* _func)(Args...))
		{
			SA_ASSERT(_caller, Nullptr, Tools, L"Add nullptr caller!");
			SA_ASSERT(_func, Nullptr, Tools, L"Add nullptr function!");

			mMemberFunctions.push_back(FuncMemberHandle{
				new FuncMemberData<C, R, Args...>{ _caller, _func },

				[](void* _data, Args... _args)
				{
					FuncMemberData<C, R, Args...>* cData = reinterpret_cast<FuncMemberData<C, R, Args...>*>(_data);

					SA_ASSERT(cData, Nullptr, Tools, L"Internal function call: data nullptr!");

					return (cData->caller->*cData->func)(_args...);
				}
			});
		}


		template <typename FuncHandle, typename FuncMemberHandle, typename R, typename... Args>
		void EventBase<FuncHandle, FuncMemberHandle, R, Args...>::Remove(R(*_func)(Args...))
		{
			SA_ASSERT(_func, Nullptr, Tools, L"Remove nullptr function!");

			for (auto it = mFunctions.begin(); it != mFunctions.end(); ++it)
			{
				if (it->func == _func)
				{
					mFunctions.erase(it);
					return;
				}
			}
		}

		template <typename FuncHandle, typename FuncMemberHandle, typename R, typename... Args>
		template <typename C>
		void EventBase<FuncHandle, FuncMemberHandle, R, Args...>::Remove(C* _caller, R(C::* _func)(Args...))
		{
			SA_ASSERT(_caller, Nullptr, Tools, L"Remove nullptr caller!");
			SA_ASSERT(_func, Nullptr, Tools, L"Remove nullptr function!");

			for (auto it = mMemberFunctions.begin(); it != mMemberFunctions.end(); ++it)
			{
				FuncMemberData<C, R, Args...>* data = reinterpret_cast<FuncMemberData<C, R, Args...>*>(it->data);

				SA_ASSERT(data, Nullptr, Tools, L"Internal function call: data nullptr!");

				if (data->caller == _caller && data->func == _func)
				{
					delete data;

					mMemberFunctions.erase(it);
					return;
				}
			}
		}


		template <typename FuncHandle, typename FuncMemberHandle, typename R, typename... Args>
		void EventBase<FuncHandle, FuncMemberHandle, R, Args...>::RRemove(R(*_func)(Args...))
		{
			SA_ASSERT(_func, Nullptr, Tools, L"RRemove nullptr function!");

			for (auto it = mFunctions.rbegin(); it != mFunctions.rend(); ++it)
			{
				if (it->func == _func)
				{
					mFunctions.erase(it);
					return;
				}
			}
		}

		template <typename FuncHandle, typename FuncMemberHandle, typename R, typename... Args>
		template <typename C>
		void EventBase<FuncHandle, FuncMemberHandle, R, Args...>::RRemove(C* _caller, R(C::* _func)(Args...))
		{
			SA_ASSERT(_caller, Nullptr, Tools, L"RRemove nullptr caller!");
			SA_ASSERT(_func, Nullptr, Tools, L"RRemove nullptr function!");

			for (auto it = mMemberFunctions.rbegin(); it != mMemberFunctions.rend(); ++it)
			{
				FuncMemberData<C, R, Args...>* data = reinterpret_cast<FuncMemberData<C, R, Args...>*>(it->data);

				SA_ASSERT(data, Nullptr, Tools, L"Internal function call: data nullptr!");

				if (data->caller == _caller && data->func == _func)
				{
					delete data;

					mMemberFunctions.erase(it);
					return;
				}
			}
		}


		template <typename FuncHandle, typename FuncMemberHandle, typename R, typename... Args>
		void EventBase<FuncHandle, FuncMemberHandle, R, Args...>::Execute(const Args&... _args)
		{
			for (auto it = mFunctions.begin(); it != mFunctions.end(); ++it)
				it->func(_args...);

			for (auto it = mMemberFunctions.begin(); it != mMemberFunctions.end(); ++it)
				it->func(it->data, _args...);
		}


		template <typename FuncHandle, typename FuncMemberHandle, typename R, typename... Args>
		void EventBase<FuncHandle, FuncMemberHandle, R, Args...>::operator+=(R(*_func)(Args...))
		{
			Add(_func);
		}

		template <typename FuncHandle, typename FuncMemberHandle, typename R, typename... Args>
		void EventBase<FuncHandle, FuncMemberHandle, R, Args...>::operator-=(R(*_func)(Args...))
		{
			Remove(_func);
		}

		template <typename FuncHandle, typename FuncMemberHandle, typename R, typename... Args>
		void EventBase<FuncHandle, FuncMemberHandle, R, Args...>::operator()(const Args&... _args)
		{
			Execute(_args...);
		}
	}


	template <typename R, typename... Args>
	void Event<R(Args...)>::Add(R(*_func)(Args...), R* _result)
	{
		SA_ASSERT(_func, Nullptr, Tools, L"Add nullptr function!");

		mFunctions.push_back(FuncHandle{ _func, _result });
	}

	template <typename R, typename... Args>
	template <typename C>
	void Event<R(Args...)>::Add(C* _caller, R(C::* _func)(Args...), R* _result)
	{
		SA_ASSERT(_caller, Nullptr, Tools, L"Add nullptr caller!");
		SA_ASSERT(_func, Nullptr, Tools, L"Add nullptr function!");

		mMemberFunctions.push_back(FuncMemberHandle{
			new Internal::FuncMemberData<C, R, Args...>{ _caller, _func },

			[](void* _data, Args... _args)
			{
				Internal::FuncMemberData<C, R, Args...>* cData = reinterpret_cast<Internal::FuncMemberData<C, R, Args...>*>(_data);

				SA_ASSERT(cData, Nullptr, Tools, L"Internal function call: data nullptr!");

				return (cData->caller->*cData->func)(_args...);
			},

			_result
		});
	}

	template <typename R, typename... Args>
	void Event<R(Args...)>::Execute(const Args&... _args)
	{
		for (auto it = mFunctions.begin(); it != mFunctions.end(); ++it)
		{
			if(it->result)
				*it->result = it->func(_args...);
			else
				it->func(_args...);
		}

		for (auto it = mMemberFunctions.begin(); it != mMemberFunctions.end(); ++it)
		{
			if(it->result)
				*it->result = it->func(it->data, _args...);
			else
				it->func(it->data, _args...);
		}
	}

	template <typename R, typename... Args>
	void Event<R(Args...)>::operator()(const Args&... _args)
	{
		Execute(_args...);
	}
}
