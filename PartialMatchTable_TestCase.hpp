#pragma once
#include "TestCase.hpp"
#include <vector>

class PartialMatchTable_TestCase : public TestCase
{
public:
    virtual void runTests();

private:
    void normal_case();

private:
    void test( const std::string& testName, const std::string& word, const std::string& expectedText );
};