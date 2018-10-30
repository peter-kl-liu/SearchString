#include "PartialMatchTable_TestCase.hpp"
#include "StringSearcher_KMP.hpp"
#include "tools.hpp"

void PartialMatchTable_TestCase::runTests()
{
    TestCase::runTests();

    normal_case();
    report_statistics();
}

void PartialMatchTable_TestCase::normal_case()
{
    test( "Web example", "abababca", "0 0 1 2 3 4 0 1" );
}

void PartialMatchTable_TestCase::test(
    const std::string& testName,
    const std::string& word,
    const std::string& expectedText )
{
    auto expected = ToVector<std::size_t>( expectedText );

    try
    {
        auto actual = StringSearcher_KMP::partial_match_table( word );

        if ( actual.size() != expected.size() )
            this->report_fail( testName, "vector size not match", expected.size(), actual.size() );

        if ( !std::equal( actual.begin(), actual.end(), expected.begin() ) )
            this->report_fail( testName, "vector contents not match" );

        this->report_success( testName );
    }
    catch ( std::exception& ex )
    {
        this->report_exception( testName, ex );
    }
    catch ( ... )
    {
        this->report_unknown_exception( testName );
    }
}
