#include "StringSearcher_Simple.hpp"

IMonitoredStringSearcher*
StringSearcher_Simple::Factory::create()
{
    return new StringSearcher_Simple();
}

StringSearcher_Simple::StringSearcher_Simple()
{
#if defined(MONITOR)
    this->from_indices_.reserve( 1000 );
    this->compare_word_indices_.reserve( 1000 );
    this->compare_from_indices_.reserve( 1000 );
#endif
}

std::size_t
StringSearcher_Simple::search( const std::string& word, const std::string& fromText ) const
{
#if defined(MONITOR)
    this->from_indices_.clear();
    this->compare_word_indices_.clear();
    this->compare_from_indices_.clear();
#endif
    if ( !word.empty() && word.length() <= fromText.length() )
    {
        std::size_t i = 0;
        for ( std::size_t m = 0; m <= fromText.length() - word.length(); ++m )
        {
            i = 0;
#if defined(MONITOR)
            // Monitor how index m jump
            this->from_indices_.push_back( m );
            this->compare_word_indices_.push_back( i );
            this->compare_from_indices_.push_back( m );
#endif
            if ( word[ i ] == fromText[ m ] )
            {
                bool found = true;
                ++i;
                while ( i < word.length() )
                {
#if defined(MONITOR)
                    // Monitor the pair of characters begin compared, also counter the number of comparision
                    this->compare_word_indices_.push_back( i );
                    this->compare_from_indices_.push_back( m + i );
#endif
                    if ( word[ i ] != fromText[ m + i ] )
                    {
                        found = false;
                        break;
                    }

                    ++i;
                }

                if ( found )
                    return m;
            }
        }
    }

    return npos;
}
