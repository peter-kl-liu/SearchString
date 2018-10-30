#pragma once
#include <sstream>
#include <vector>

template <typename T>
std::vector<T> ToVector( const std::string& text )
{
    std::vector<T> result;

    T value;
    std::stringstream s( text );
    while ( s >> value )
        result.push_back( value );

    return result;
}

template <typename T>
std::string ToString( const std::vector<T>& vectors, const std::string& sep = ", " )
{
    std::ostringstream result;

    for ( auto it = vectors.begin(); it != vectors.end(); ++it )
        result << ( it != vectors.begin() ? sep : "" ) << *it;

    return result.str();
}
