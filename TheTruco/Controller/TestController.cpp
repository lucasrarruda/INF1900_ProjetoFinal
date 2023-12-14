#include "pch.h"
#include "TestController.h"

TestController::TestController(const int& test)
{
	_testModel = TestModel(test);
}

int TestController::getTest()
{
	return _testModel.getTest();
}
