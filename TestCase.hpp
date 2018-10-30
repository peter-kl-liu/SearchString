#pragma once
#include <chrono>
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>

class TestCase
{
public:
    virtual void runTests()
    {
        this->clear_statistics();
    }

protected:
    void report_success( const std::string& testName, const std::string& comments = "" )
    {
        ++this->count_tests_;
        ++this->count_success_;

        std::cout << testCase_title( testName ) << " : OK  " << std::endl;
    }

    void report_success( const std::string& testName, const std::string& comments, std::size_t iterations, double duration_ms )
    {
        ++this->count_tests_;
        ++this->count_success_;

        std::cout << testCase_title( testName );
        std::cout << " : OK  ";
        std::cout << " - Time";
        std::cout << " is " << std::right << std::setw(12) << std::fixed << std::setprecision(6) << duration_ms << " ms";
        std::cout << " " << comments;
        if ( iterations > 1 )
            std::cout << " for " << iterations << " iterations";
        std::cout << std::endl;
    }

    void report_fail( const std::string& testName, const std::string& comments = "", std::size_t iteration = 1 )
    {
        ++this->count_tests_;
        ++this->count_fail_incorrect_result_;

        std::cout << testCase_title( testName )
                  << " : Fail @ iteration(" << iteration << ")"
                  << " " << comments
                  << std::endl;
    }

    // function template
    template <typename ResultType>
    void report_fail( const std::string& testName, const std::string& comments, const ResultType& expected, const ResultType& actual, std::size_t iteration = 1 )
    {
        ++this->count_tests_;
        ++this->count_fail_incorrect_result_;

        std::cout << testCase_title( testName )
                  << " : Fail @ iteration(" << iteration << ")"
                  << " " << comments
                  << " - Expected: " << '"' << expected << '"' << " Actual: " << '"' << actual << '"'
                  << std::endl;
    }

    void report_exception( const std::string& testName, const std::exception& ex, std::size_t iteration = 1 )
    {
        ++this->count_tests_;
        ++this->count_fail_exception_;

        std::cout << testCase_title( testName )
                  << " : Fail @ iteration(" << iteration << ")"
                  << " - Exception " << ex.what()
                  << std::endl;
    }

    void report_unknown_exception( const std::string& testName, std::size_t iteration = 1 )
    {
        ++this->count_tests_;
        ++this->count_fail_unknown_exception_;

        std::cout << testCase_title( testName )
                  << " : Fail @ iteration(" << iteration << ")"
                  << " - Unknown exception"
                  << std::endl;
    }

    void report_statistics()
    {
        std::cout << "- #Success: " << this->count_success_ << std::endl;

        std::cout << "- #Fail:    " << this->count_fail();
        if ( this->count_fail() != 0 )
        {
            std::cout << " (incorrect/exception/unknown_exception)"
                      << " ( "
                      << this->count_fail_incorrect_result_ << " / "
                      << this->count_fail_exception_ << " / "
                      << this->count_fail_unknown_exception_
                      << " )";
        }
        std::cout << std::endl << std::endl;
    }

private:
    std::string testCase_title( const std::string& testName ) const
    {
        std::ostringstream s;

        s << "#" << std::right << std::setw(3) << this->count_tests_ << ": "
          << " " << std::left  << std::setw(testNameWidth) << testName;

        return s.str();
    }

    void clear_statistics()
    {
        this->count_tests_ = 0;
        this->count_success_ = 0;
        this->count_fail_incorrect_result_ = 0;
        this->count_fail_exception_ = 0;
        this->count_fail_unknown_exception_ = 0;
    }

    std::size_t count_fail() const
    {
        return this->count_fail_incorrect_result_
             + this->count_fail_exception_
             + this->count_fail_unknown_exception_;
    }

private:
    static const std::size_t testNameWidth = 50;

private:
    mutable std::size_t count_tests_;
    mutable std::size_t count_success_;
    mutable std::size_t count_fail_incorrect_result_;
    mutable std::size_t count_fail_exception_;
    mutable std::size_t count_fail_unknown_exception_;
};
