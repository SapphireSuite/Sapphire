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
	//auto lambda2 = [&a](int i) { LOG("A Lambda: " << i) a.Pim(i); };
	
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



	LOG("\n=== End ===");

	return 0;
}
