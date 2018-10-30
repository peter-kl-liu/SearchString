#pragma once

template <typename T>
class IFactory
{
public:
    virtual ~IFactory() { }

public:
    virtual T* create() = 0;
};