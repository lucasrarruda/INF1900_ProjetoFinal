#include "pch.h"
#include "TestModel.h"

TestModel::TestModel(const int& test): _test(test)
{
}

int TestModel::getTest()
{
    return _test;
}
