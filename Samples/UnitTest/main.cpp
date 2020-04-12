// Copyright 2020 Sapphire development team. All Rights Reserved.

#include "UnitTest.hpp"

#include <Sapphire/Core/Time/DateTime.hpp>
#include <Sapphire/Collections/Algorithms>
using namespace Sa;

int main()
{
	LOG("=== Start ===");


	try
	{
		MemCopy<int>(nullptr, nullptr, 3);
	}
	catch (const Exception<ExceptCode::Nullptr>& _exception)
	{
		std::wcout << "----- Nullptr Exception caught -----" << std::endl;

		std::wcout << _exception << std::endl;
	}
	catch (...)
	{
		// Exception-catch failed.
		return 1;
	}


	try
	{
		DateTime time;
		time.weekDay = 89;

		time.GetDayName();
	}
	catch (const Exception<ExceptCode::OutOfRange>& _exception)
	{
		std::wcout << "----- OutOfRange Exception caught -----" << std::endl;

		std::wcout << _exception << std::endl;
	}
	catch (...)
	{
		// Exception-catch failed.
		return 1;
	}


	LOG("\n=== End ===");

	return UnitTest::Exit;
}
