#include <TestModel.h>

#pragma once
class TestController
{
public:
	TestController(const int& test);
	~TestController() = default;

	int getTest();
private:
	TestModel _testModel;
};

