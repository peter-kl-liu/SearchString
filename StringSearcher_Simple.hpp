#pragma once
#include "IMonitoredStringSearcher.hpp"

class StringSearcher_Simple : public IMonitoredStringSearcher
{
public:
    class Factory : public IMonitoredStringSearcherFactory
    {
        virtual IMonitoredStringSearcher* create();
    };

public:
    StringSearcher_Simple();

    // interface IStringSearcher
    virtual std::size_t search( const std::string& word, const std::string& fromText ) const;

public:
    // interface IMonitoredStringSearcher
    virtual std::size_t                     count_compare() const        { return this->compare_from_indices_.size(); }
    virtual const std::vector<std::size_t>& from_indices() const         { return this->from_indices_; }
    virtual const std::vector<std::size_t>& compare_word_indices() const { return this->compare_word_indices_; }
    virtual const std::vector<std::size_t>& compare_from_indices() const { return this->compare_from_indices_; }

private:
    mutable std::vector<std::size_t> from_indices_;
    mutable std::vector<std::size_t> compare_word_indices_;
    mutable std::vector<std::size_t> compare_from_indices_;
};
