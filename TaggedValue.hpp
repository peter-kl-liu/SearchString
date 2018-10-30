#pragma once

template <typename T, int N>
struct TaggedValue
{
    explicit TaggedValue( const T& v ) : value( v ) { }
    T value;
};
