#pragma once
class TestModel
{
public:
	TestModel() = default;
	TestModel(const int& test);
	~TestModel() = default;

	int getTest();
private:
	int _test;
};

