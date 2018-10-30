#pragma once
#include <limits>
#include <vector>

template <typename T>
class ISearcher
{
public:
    static const std::size_t npos = -1;

public:
    virtual ~ISearcher() { }

public:
    // Search for the first 'pattern' inside 'from', return npos if not found
    virtual std::size_t search( const T& pattern, const T& from ) const = 0;
};