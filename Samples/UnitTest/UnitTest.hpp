// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_UNIT_TEST_GUARD
#define SAPPHIRE_UNIT_TEST_GUARD

#include <cstring>

#include <string>
#include <iostream>

#include <Core/Types/Int.hpp>

#define LOG(_str) std::cout << _str << std::endl;

namespace Sa
{
	/**
	*	\file UnitTest.hpp
	*
	*	\brief \b Definition of Sapphire's Unit Testing tool.
	*
	*	\ingroup Tools
	*	\{
	*/

	/**
	*	\brief Sapphire's Unit Testing tool class.
	*/
	class UnitTest
	{
	public:
		/**
		*	\brief Exit result of unit testing.
		*
		*	Exit 0 == success.
		*	Exit 1 == failure.
		*/
		static uint32 Exit;

		static constexpr uint32 TestNum = 10u;
	};


	/**	\} */
}


/**
*	\brief Run a \e <em> Unit Test </em>.
*
*	UnitTestExit will be equal to 1 if at least one test failed.
*
*	\param[in] _lhs		Left hand side operand to test.
*	\param[in] _op		Operator of the test between _lhs and _rhs.
*	\param[in] _rhs		Right hand side operand to test.
*/
#define SA_TEST(_lhs, _op, _rhs)\
{\
	auto lhsValue = _lhs;\
	auto rhsValue = _rhs;\
	bool bRes = lhsValue _op rhsValue;\
\
	LOG("Test:\t\t" << #_lhs " " #_op " " #_rhs << " -- [" << lhsValue << "] "#_op " [" << rhsValue << "]:");\
	LOG((bRes ? "Success\n" : "Failure\n"))\
\
	if(!bRes)\
		Sa::UnitTest::Exit = 1;\
}

/**
*	\brief Run a <em> Unit Try-Test </em>.
*
*	Try-test does not impact UnitTestExit.
*
*	\param[in] _lhs		Left hand side operand to test.
*	\param[in] _op		Operator of the test between _lhs and _rhs.
*	\param[in] _rhs		Right hand side operand to test.
*/
#define SA_TRY_TEST(_lhs, _op, _rhs)\
{\
	auto lhsValue = _lhs;\
	auto rhsValue = _rhs;\
	bool bRes = lhsValue _op rhsValue;\
\
	LOG("Try Test:\t" << #_lhs " " #_op " " #_rhs << " -- [" << lhsValue << "] "#_op " [" << rhsValue << "]:");\
	LOG((bRes ? "Success\n" : "Failure\n"))\
}

#endif // GUARD
