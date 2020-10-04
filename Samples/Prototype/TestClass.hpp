// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#include "TestClass.gen"

SA_CLASS()
class TestClass : public Sa::Object
{
	SA_REFLECT();

public:

	SA_PROP();
	int x;

	SA_PROP();
	std::string str;
};