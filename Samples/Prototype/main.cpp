// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <string>
#include <iostream>

#include <Sapphire/Collections/Types>
using namespace Sa;

#include "TestClass.hpp"


#define LOG(_str) std::cout << _str << std::endl;


int main()
{
	LOG("=== Start ===");


	TestClass test1;

	test1.x = 7;
	test1.str = "Hello, world!";

	LOG(test1.x);
	LOG(test1.str);

	const Sa::Class& aa = test1.TypeInfos();

	//LOG("\n=== Serialize ===");
	//std::string testSrl = test1.Serialize();

	//TestClass replic;
	//replic.Deserialize(testSrl);

	//LOG(replic.x);
	//LOG(replic.str);


	LOG("\n=== End ===");

	return 0;
}
