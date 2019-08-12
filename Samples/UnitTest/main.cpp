// Copyright 2020 Sapphire development team. All Rights Reserved.

#include "UnitTest.hpp"

#include <Sapphire/Core/Types/Variadics/StaticConstant.hpp>
using namespace Sa;

int main()
{
	LOG("=== Start ===");


	SA_TEST((StaticConstant<bool, true>::value), ==, true);

	SA_TEST((StaticConstant<bool, false>::value), ==, false);

	SA_TEST((StaticConstant<char, 'w'>::value), ==, 'w');

	SA_TEST((StaticConstant<int, -893>::value), ==, -893);


	LOG("\n=== End ===");

	return UnitTest::Exit;
}
