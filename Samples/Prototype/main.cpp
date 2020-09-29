// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <string>
#include <iostream>

#include <Sapphire/Collections/Types>
#include <Sapphire/Core/Types/Variadics/Event.hpp>
using namespace Sa;

#define LOG(_str) std::cout << _str << std::endl;

void Foo(int i)
{
	LOG("Foo: " << i);
}

void Bar(int i)
{
	LOG("Bar: " << i);
}

struct A
{
	void Pim(int i)
	{
		LOG("A::Pim: " << i);
	}
};

int main()
{
	LOG("=== Start ===");


	Event<void(int)> event;


	LOG("=== 1 ===");

	event.Add(Foo);
	event.Add(Bar);

	event.Execute(1);


	LOG("=== 2 ===");
	
	event.Remove(Foo);
	
	event.Execute(2);


	LOG("=== 3 ===");

	auto lambda1 = [](int i) { LOG("Lambda: " << i) };
	
	event.Add(lambda1);

	event.Execute(3);


	LOG("=== 4 ===");

	event.Remove(lambda1);

	event.Execute(4);


	LOG("=== 5 ===");

	A a;
	
	event.Add(&a, &A::Pim);

	event.Execute(5);


	LOG("=== 6 ===");

	event.Remove(&a, &A::Pim);

	event.Execute(6);


	LOG("=== 7 ===");

	Event<int(int)> event2;

	auto lambda2 = [](int i) { LOG("R Lambda: " << i) return i * 2; };
	int res = 7;

	event2.Add(lambda2);
	event2.Add(lambda2, &res);


	event2(res);

	LOG("Res: " << res)

	event2.Execute(res);

	LOG("Res: " << res)

	event2.Remove(lambda2);


	LOG("\n=== End ===");

	return 0;
}
