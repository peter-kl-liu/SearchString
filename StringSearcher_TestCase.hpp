#pragma once
#include "IMonitoredStringSearcher.hpp"
#include "TaggedValue.hpp"
#include "TestCase.hpp"
#include <fstream>
#include <memory>
#include <string>
#include <vector>

class StringSearcher_TestCase : public TestCase
{
public:
    StringSearcher_TestCase( const std::string& name, std::unique_ptr<IMonitoredStringSearcherFactory> factory );

public:
    virtual void runTests();

private:
    void handle_empty_string();
    void search_single_char();
    void return_first_occurrence_only();
    void web_examples();
    void favor_KMP();
    void worst_cases();

private:
    typedef TaggedValue<std::size_t,0> RepeatCount;

    void testSearch(
        const std::string&  testName,
        const std::string&  word,
        const std::string&  fromText,
        std::size_t         expectedResult,
        RepeatCount         max_iterations = RepeatCount( 1 ) );

private:
    const std::string                                name_;
    std::ofstream                                    trace_;
    std::unique_ptr<IMonitoredStringSearcherFactory> factory_;

    void traceSearch(
        const std::string&              testName,
        const std::string&              word,
        const std::string&              fromText,
        std::size_t                     expectedResult,
        std::size_t                     actualResult,
        const std::vector<std::size_t>& from_indices,
        const std::vector<std::size_t>& compare_word_indices,
        const std::vector<std::size_t>& compare_from_indices );
};