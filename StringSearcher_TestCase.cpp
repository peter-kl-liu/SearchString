#include "StringSearcher_TestCase.hpp"
#include "IMonitoredStringSearcher.hpp"
#include "StringSearcher_KMP.hpp"
#include "tools.hpp"
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace
{
    const auto npos = IStringSearcher::npos;

    std::string ruler_string( std::size_t length, std::size_t scale = 1 )
    {
        std::ostringstream s;
        for (std::size_t mark = 0; mark < length; ++mark )
            if ( scale == 1 || mark % scale == 0 )
                s << ( ( mark / scale ) % 10 );
            else
                s << ' ';
        return s.str();
    }
}

StringSearcher_TestCase::StringSearcher_TestCase( const std::string& name, std::unique_ptr<IMonitoredStringSearcherFactory> factory )
: name_( name ),
  factory_( std::move( factory ) )
{
}

void StringSearcher_TestCase::runTests()
{
    TestCase::runTests();

    auto trace_file = this->name_ + ".txt";
    this->trace_.open( trace_file, std::ios::trunc );

    handle_empty_string();
    search_single_char();
    return_first_occurrence_only();
    web_examples();
    favor_KMP();
    worst_cases();
    // TODO: Add more boundary test cases

    report_statistics();
    this->trace_.close();
}

void StringSearcher_TestCase::handle_empty_string()
{
    testSearch( "When word is empty"    , ""    , "A" , npos );
    testSearch( "When from is empty"    , "W"   , ""  , npos );
    testSearch( "When both are empty"   , ""    , ""  , npos );
}

void StringSearcher_TestCase::search_single_char()
{
    testSearch( "Found when char appear at all positions" , "A"   , "AAAA"  , 0 );
    testSearch( "Found when char appear at middle"        , "A"   , "BABB"  , 1 );
    testSearch( "Found when char appear at end"           , "A"   , "BBBA"  , 3 );
    testSearch( "Single char not found"                   , "A"   , "BCDE"  , npos );
}

void StringSearcher_TestCase::return_first_occurrence_only()
{
    testSearch( "Return first occurrence of A"    , "A"     , "BABABCDEFG A"  , 1 );
    testSearch( "Return first occurrence of AB"   , "AB"    , "ABABABAB AB "  , 0 );
}

void StringSearcher_TestCase::web_examples()
{
    //                                                                 1         2
    //                                          0123456789   01234567890123456789012
    testSearch( "Found ABCDABD at last"      , "ABCDABD"  , "ABC ABCDAB ABCDABCDABDE", 15 );
}


void StringSearcher_TestCase::favor_KMP()
{
    //                                                                       1         2         3         4         5          6
    //                                             0123456789      0123456789012345678901234567890123456789012345678901234567890123
    testSearch( "Found BAAAAAAAAA at last"      , "BAAAAAAAAA"  , "BAAAAAAAABAAAAAAAABAAAAAAAABAAAAAAAABAAAAAAAABAAAAAAAABAAAAAAAAA", 54, RepeatCount(10000) );
}

void StringSearcher_TestCase::worst_cases()
{
    //                                                                       1         2         3         4         5
    //                                             0123456789      012345678901234567890123456789012345678901234567890123456789
    testSearch( "Found AAAAAAAAAB at last"      , "AAAAAAAAAB"  , "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAB", 50, RepeatCount(10000) );
}

void StringSearcher_TestCase::testSearch(
    const std::string&  testName,
    const std::string&  word,
    const std::string&  fromText,
    std::size_t         expectedResult,
    RepeatCount         max_iterations )
{
    std::unique_ptr<IMonitoredStringSearcher> subject( this->factory_->create() );

    double duration_ns = 0;
    std::size_t count_compare = 0;
    std::size_t iteration = 0;
    try
    {
        for ( iteration = 1; iteration <= max_iterations.value; ++iteration )
        {
            auto start = std::chrono::high_resolution_clock::now();

            std::size_t index = subject->search( word, fromText );

            auto finish = std::chrono::high_resolution_clock::now();
            auto ellapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>( finish - start ).count();
            duration_ns += ellapsed_ns;

            if ( iteration == 1 )
            {
                count_compare = subject->count_compare();
                this->traceSearch( testName, word, fromText, expectedResult, index, subject->from_indices(), subject->compare_word_indices(), subject->compare_from_indices() );
            }

            if ( index != expectedResult )
            {
                this->report_fail( testName, "", expectedResult, index, iteration );
                return;
            }
        }
    }
    catch ( std::exception& ex )
    {
        this->report_exception( testName, ex, iteration );
        return;
    }
    catch ( ... )
    {
        this->report_unknown_exception( testName, iteration );
        return;
    }

    std::ostringstream comment;
    comment << "#Compare = " << std::right << std::setw(3) << count_compare;

    this->report_success( testName, comment.str(), max_iterations.value, duration_ns / 1000 / 1000 );
}

void StringSearcher_TestCase::traceSearch(
    const std::string&              testName,
    const std::string&              word,
    const std::string&              fromText,
    std::size_t                     expectedResult,
    std::size_t                     actualResult,
    const std::vector<std::size_t>& from_indices,
    const std::vector<std::size_t>& compare_word_indices,
    const std::vector<std::size_t>& compare_from_indices )
{
    if ( this->trace_ )
    {
        auto table = StringSearcher_KMP::partial_match_table( word );
        {
            this->trace_ << testName << ":" << std::endl
                         << "- Searching"   << std::endl
                         << "-   ruler:    " << ' ' << ruler_string( fromText.length(), 10 ) << std::endl
                         << "-             " << ' ' << ruler_string( fromText.length(),  1 ) << std::endl
                         << "-   fromText: " << '"' << fromText << '"' << std::endl
                         << "-   word:     " << '"' << word     << '"' << std::endl
                         << "-   partial match table: " << '[' << ToString( table, "," ) << ']' << std::endl
                         << "-   expect pos:  " << '"' << expectedResult << '"' << std::endl
                         << "-   actual pos:  " << '"' << actualResult   << '"' << std::endl
                         << "-   fromText indices history: " << '[' << ToString( from_indices, "," ) << ']' << std::endl;
        }
        for ( std::size_t step = 0; step < compare_from_indices.size(); ++step )
        {
            std::size_t m = compare_from_indices[ step ];
            std::size_t i = compare_word_indices[ step ];

            this->trace_ << "-   Step #" << std::right << std::setw(3) << ( step + 1 )
                         << " compare word"  << '[' << std::right << std::setw(3) << i << ']' << "('" << word[i]     << "')"
                         << " with fromText" << '[' << std::right << std::setw(3) << m << ']' << "('" << fromText[m] << "')"
                         << " " << ( word[i] == fromText[m] ? "" : "X" )
                         << std::endl;
        }
    }

    this->trace_ << std::endl;
    this->trace_ << std::endl;
}
