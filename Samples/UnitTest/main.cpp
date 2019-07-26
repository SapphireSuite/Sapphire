// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <type_traits>

#include "UnitTest.hpp"

#include <Sapphire/Core/Types/Extractions/RawType.hpp>
#include <Sapphire/Core/Types/Extractions/FunctionType.hpp>
using namespace Sa;

struct A
{
	void Foo(int i, float f)
	{
		(void)i;
		(void)f;
	}
};

int main()
{
	LOG("=== Start ===");


	SA_TEST((std::is_same<RemoveRef<int>, int>::value), ==, true);
	SA_TEST((std::is_same<RemoveRef<int&>, int>::value), ==, true);
	SA_TEST((std::is_same<RemoveRef<int&&>, int>::value), ==, true);
	SA_TEST((std::is_same<RemoveRef<int*&>, int*>::value), ==, true);

	SA_TEST((std::is_same<RemoveConstVol<int>, int>::value), ==, true);
	SA_TEST((std::is_same<RemoveConstVol<const int>, int>::value), ==, true);
	SA_TEST((std::is_same<RemoveConstVol<const volatile int>, int>::value), ==, true);
	SA_TEST((std::is_same<RemoveConstVol<int* const>, int*>::value), ==, true);
	SA_TEST((std::is_same<RemoveConstVol<const int* const>, const int*>::value), ==, true);

	SA_TEST((std::is_same<RemovePtr<int>, int>::value), ==, true);
	SA_TEST((std::is_same<RemovePtr<int*>, int>::value), ==, true);
	SA_TEST((std::is_same<RemovePtr<const int*>, const int>::value), ==, true);
	SA_TEST((std::is_same<RemovePtr<int* const*>, int* const>::value), ==, true);
	SA_TEST((std::is_same<RemovePtr<int*&>, int*&>::value), ==, true);

	SA_TEST((std::is_same<RawType<int>, int>::value), ==, true);
	SA_TEST((std::is_same<RawType<int*>, int>::value), ==, true);
	SA_TEST((std::is_same<RawType<int**>, int>::value), ==, true);
	SA_TEST((std::is_same<RawType<const int**&>, int>::value), ==, true);
	SA_TEST((std::is_same<RawType<const int* const*&>, int>::value), ==, true);

	SA_TEST((std::is_same<FunctionType<decltype(&A::Foo)>::ReturnT, void>::value), ==, true);
	SA_TEST((std::is_same<FunctionType<decltype(&A::Foo)>::FuncT, void(A::*)(int, float)>::value), ==, true);
	SA_TEST((std::is_same<FunctionType<decltype(&A::Foo)>::RawFuncT, void(*)(int, float)>::value), ==, true);


	LOG("\n=== End ===");

	return UnitTest::Exit;
}
