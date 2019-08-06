// Copyright 2020 Sapphire development team. All Rights Reserved.

#include "UnitTest.hpp"

#include <Sapphire/Core/Types/Conditions/IsSame.hpp>
#include <Sapphire/Core/Types/Conditions/IsConst.hpp>
#include <Sapphire/Core/Types/Conditions/IsScalar.hpp>
#include <Sapphire/Core/Types/Conditions/IsSigned.hpp>
#include <Sapphire/Core/Types/Conditions/IsFunction.hpp>
#include <Sapphire/Core/Types/Conditions/IsNative.hpp>
using namespace Sa;

template <typename T>
struct A
{
};

template <typename T>
struct B
{
};

template <typename T1, typename T2>
struct C
{
};

struct D
{
	void Foo()
	{
	}
};

void Bar()
{
}

int main()
{
	LOG("=== Start ===");


	LOG("\n=== IsSame ===");
	SA_TEST((IsSame<int32, int32>::value), ==, true);
	SA_TEST((IsSame<int32, float>::value), ==, false);
	SA_TEST((IsSame<int32*, int32>::value), ==, false);

	SA_TEST((IsSame<A<int32>, A<int32>>::value), ==, true);
	SA_TEST((IsSame<A<int32>, B<int32>>::value), ==, false);

	SA_TEST((IsSameTmpl<A, A>::value), ==, true);
	SA_TEST((IsSameTmpl<A, B>::value), ==, false);
	SA_TEST((IsSameTmpl<A, C>::value), ==, false);


	LOG("\n=== IsConst ===");
	SA_TEST((IsConst<const int32>::value), ==, true);
	SA_TEST((IsConst<int32>::value), ==, false);


	LOG("\n=== IsScalar ===");
	SA_TEST((IsScalar<int32>::value), ==, true);
	SA_TEST((IsScalar<int32&>::value), ==, false);
	SA_TEST((IsScalar<int32*>::value), ==, false);
	SA_TEST((IsScalar<int32[]>::value), ==, false);

	LOG("\n=== IsSigned ===");
	SA_TEST((IsSigned<int32>::value), ==, true);
	SA_TEST((IsSigned<uint32>::value), ==, false);
	SA_TEST((IsSigned<int32*>::value), ==, true);


	LOG("\n=== IsUnsigned ===");
	SA_TEST((IsUnsigned<uint32>::value), ==, true);
	SA_TEST((IsUnsigned<int32>::value), ==, false);
	SA_TEST((IsUnsigned<uint32*>::value), ==, true);


	LOG("\n=== IsPtr ===");
	SA_TEST((IsPointer<int32*>::value), ==, true);
	SA_TEST((IsPointer<int32>::value), ==, false);
	SA_TEST((IsPointer<int32&>::value), ==, false);
	SA_TEST((IsPointer<const int32*>::value), ==, true);
	SA_TEST((IsPointer<int32[]>::value), ==, false);


	LOG("\n=== IsReference ===");
	SA_TEST((IsReference<int32&>::value), ==, true);
	SA_TEST((IsReference<int32*>::value), ==, false);
	SA_TEST((IsReference<int32&&>::value), ==, false);
	SA_TEST((IsReference<int32>::value), ==, false);
	SA_TEST((IsReference<const int32&>::value), ==, true);
	SA_TEST((IsReference<int32[]>::value), ==, false);


	LOG("\n=== IsRValue ===");
	SA_TEST((IsRValue<int32&&>::value), ==, true);
	SA_TEST((IsRValue<int32*>::value), ==, false);
	SA_TEST((IsRValue<int32&>::value), ==, false);
	SA_TEST((IsRValue<int32>::value), ==, false);
	SA_TEST((IsRValue<int32[]>::value), ==, false);


	LOG("\n=== IsArray  ===");
	SA_TEST((IsArray<int32[]>::value), ==, true);
	SA_TEST((IsArray<const int32[]>::value), ==, true);
	SA_TEST((IsArray<int32*>::value), ==, false);
	SA_TEST((IsArray<int32>::value), ==, false);


	LOG("\n=== IsFunction  ===");
	SA_TEST((IsFunction<int32>::value), ==, false);
	SA_TEST((IsFunction<decltype(&Bar)>::value), ==, true);
	SA_TEST((IsFunction<decltype(&D::Foo)>::value), ==, true);


	LOG("\n=== IsIntegral  ===");
	SA_TEST((IsIntegral<int32>::value), ==, true);
	SA_TEST((IsIntegral<uint32>::value), ==, true);
	SA_TEST((IsIntegral<float>::value), ==, false);
	SA_TEST((IsIntegral<int32*>::value), ==, false);


	LOG("\n=== IsFloatingPoint  ===");
	SA_TEST((IsFloatingPoint<float>::value), ==, true);
	SA_TEST((IsFloatingPoint<double>::value), ==, true);
	SA_TEST((IsFloatingPoint<int32>::value), ==, false);
	SA_TEST((IsFloatingPoint<float*>::value), ==, false);


	LOG("\n=== IsArithmetic  ===");
	SA_TEST((IsArithmetic<int32>::value), ==, true);
	SA_TEST((IsArithmetic<float>::value), ==, true);
	SA_TEST((IsArithmetic<int32*>::value), ==, false);
	SA_TEST((IsArithmetic<D>::value), ==, false);


	LOG("\n=== IsNative  ===");
	SA_TEST((IsNative<int32>::value), ==, true);
	SA_TEST((IsNative<float>::value), ==, true);
	SA_TEST((IsNative<int32*>::value), ==, true);
	SA_TEST((IsNative<int32&>::value), ==, true);
	SA_TEST((IsNative<D>::value), ==, false);


	LOG("\n=== End ===");

	return UnitTest::Exit;
}
