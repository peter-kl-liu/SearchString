#include "PartialMatchTable_TestCase.hpp"
#include "StringSearcher_KMP.hpp"
#include "StringSearcher_Simple.hpp"
#include "StringSearcher_TestCase.hpp"
#include <iostream>

int main( int argc, char* argv[] )
{
    {
        std::cout << "Test PartialMatchTable:" << std::endl;
        PartialMatchTable_TestCase testCase;
        testCase.runTests();
    }
    {
        std::cout << "Test StringSearcher_Simple:" << std::endl;
        StringSearcher_TestCase testCase( "Test_StringSearcher_Simple", std::unique_ptr<IMonitoredStringSearcherFactory>( new StringSearcher_Simple::Factory() ) );
        testCase.runTests();
    }
    {
        std::cout << "Test StringSearcher_KMP:" << std::endl;
        StringSearcher_TestCase testCase( "Test_StringSearcher_KMP", std::unique_ptr<IMonitoredStringSearcherFactory>( new StringSearcher_KMP::Factory() ) );
        testCase.runTests();
    }

    return 0;
}
