// Copyright 2020 Sapphire development team. All Rights Reserved.

#include "UnitTest.hpp"

#include <Sapphire/Collections/Support>
#include <Sapphire/Dummy.hpp>
using namespace Sa;

int main()
{
	LOG("=== Start ===");


	LOG("Hello, world!");

	// Test DLL linkage.
	Dummy d;

	LOG("Engine Version: " << SA_VERSION_MAJOR << '.' << SA_VERSION_MINOR);

#if SA_DEBUG
	LOG("Configuration: Debug");
#else
	LOG("Configuration: Release");
#endif

#if SA_x32
	LOG("Architecture: x32");
#elif SA_x64
	LOG("Architecture: x64");
#endif

#if SA_ARM
	LOG("ARM Architecture");
#endif


#if SA_WIN
	LOG("Platform: Windows");
#elif SA_ANDROID
	LOG("Platform: Android");
#elif SA_UNIX
	LOG("Platform: Linux");
#elif SA_OSX
	LOG("Platform: Apple");
#else
	LOG("Platform: Unknown");
	return 1; // Failure.
#endif


#if SA_MSVC
	LOG("Compiler: MSVC");
#elif SA_CLANG
	LOG("Compiler: Clang");
#elif SA_GNU
	LOG("Compiler: GNU");
#else
	LOG("Compiler: Unknown");
	return 1; // Failure.
#endif


	LOG("\n=== End ===");

	return UnitTest::Exit;
}
