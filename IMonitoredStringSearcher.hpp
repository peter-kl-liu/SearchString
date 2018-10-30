#pragma once
#include "IStringSearcher.hpp"

class IMonitoredStringSearcher : public IStringSearcher
{
public:
    virtual std::size_t                     count_compare() const = 0;
    virtual const std::vector<std::size_t>& from_indices() const = 0;
    virtual const std::vector<std::size_t>& compare_word_indices() const = 0;
    virtual const std::vector<std::size_t>& compare_from_indices() const = 0;
};

typedef IFactory<IMonitoredStringSearcher> IMonitoredStringSearcherFactory;
