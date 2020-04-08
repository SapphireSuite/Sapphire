// Copyright 2020 Sapphire development team. All Rights Reserved.

#include "UnitTest.hpp"

#include <Sapphire/Collections/Algorithms>
using namespace Sa;

int main()
{
	LOG("=== Start ===");


	LOG("\n=== MemFill ===");
	char str[6]{};
	MemFill(str, 'y', 5);
	SA_TEST(strcmp(str, "yyyyy"), == , 0);

	int tab1[5];
	MemFill(tab1, 808, 5);
	SA_TEST(tab1[0], ==, 808);
	SA_TEST(tab1[1], ==, 808);
	SA_TEST(tab1[2], ==, 808);
	SA_TEST(tab1[3], ==, 808);
	SA_TEST(tab1[4], ==, 808);


	LOG("\n=== MemCopy ===");
	int tab2[5];
	MemCopy(tab1, tab2, 5);

	SA_TEST(tab2[0], == , 808);
	SA_TEST(tab2[1], == , 808);
	SA_TEST(tab2[2], == , 808);
	SA_TEST(tab2[3], == , 808);
	SA_TEST(tab2[4], == , 808);


	LOG("\n=== MemMove ===");
	int tab3[5];
	MemMove(tab2, tab3, 5);
	SA_TEST(tab3[0], == , 808);
	SA_TEST(tab3[1], == , 808);
	SA_TEST(tab3[2], == , 808);
	SA_TEST(tab3[3], == , 808);
	SA_TEST(tab3[4], == , 808);


	LOG("\n=== MemReset ===");
	MemReset(str, 5);
	SA_TEST(str[0], == , 0);
	SA_TEST(str[1], == , 0);
	SA_TEST(str[2], == , 0);
	SA_TEST(str[3], == , 0);
	SA_TEST(str[4], == , 0);
	SA_TEST(str[5], == , 0);


	LOG("\n=== IsNull ===");
	int i1 = 0; int i2 = 0; int i3 = 0;

	SA_TEST(IsNull(&i1, &i2, &i3), == , false);
	SA_TEST(IsNull(&i1, &i2, nullptr, &i3), == , true);


	LOG("\n=== Convert ===");

	wchar wA;
	Convert('a', wA);
	SA_TEST(wA, ==, L'a');

	char8 cB;
	Convert(L'b', cB);
	SA_TEST(cB, == , 'b');

	char8 c780;
	Convert(780, c780, '?');
	SA_TEST(c780, == , '?');

	const wchar wABCD[6]{ L'A', 780 , L'B' , L'C' , L'D', L'\0' };
	char8 cABCD[6];
	Convert(wABCD, cABCD, '?');
	SA_TEST(cABCD[0], == , 'A');
	SA_TEST(cABCD[1], == , '?');
	SA_TEST(cABCD[2], == , 'B');
	SA_TEST(cABCD[3], == , 'C');
	SA_TEST(cABCD[4], == , 'D');


	LOG("\n=== End ===");

	return UnitTest::Exit;
}
