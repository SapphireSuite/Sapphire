// Copyright 2020 Sapphire development team. All Rights Reserved.

#include "UnitTest.hpp"

#include <Sapphire/Collections/Debug>
using namespace Sa;

SA_LOG_CHAN_DEFINE(SuperChan)

int main()
{
	LOG("=== Start ===");


	LOG("\n=== Log ===");
	SA_LOG("Hello, World!");

	SA_LOG("Hello, Infos World!", Infos);

	SA_LOG("Hello, Super Warning World!", Warning, SuperChan);

	SA_LOG_LVL_ENABLE(Infos, false);
	SA_LOG("Try log in a disabled level", Infos);


	SA_LOG_CHAN_ENABLE_LOG(SuperChan, false)
	SA_LOG("Try Log in a disabled channel!", Normal, SuperChan);

	SA_LOG("Try log in a non-registered channel", Normal, NonRegisteredChannel);


	SA_LOG_CHAN_ENABLE_LOG(SuperChan, true)
	SA_LOG("Channel is back to enabled nut not level", Infos, SuperChan);

	SA_LOG_LVL_ENABLE(Infos, true);
	SA_LOG("Everything is fine!", Infos, SuperChan);


	LOG("\n=== Assertion ===");
	SA_ASSERT(true, Unknown, Default);

	try
	{
		SA_ASSERT(false, Unknown, Default, L"Try catch assertion");
	}
	catch (const Exception<ExceptCode::Unknown>& _exception)
	{
		std::wcout << "----- Unknown Exception caught -----" << std::endl;

		_exception.Output(std::wcout);
	}
	catch (const ExceptionBase& _exception)
	{
		std::wcout << "----- Exception caught -----" << std::endl;

		_exception.Output(std::wcout);
	}


	LOG("\n=== End ===");

	return UnitTest::Exit;
}
