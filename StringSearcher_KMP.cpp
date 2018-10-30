// http://jakeboxer.com/blog/2009/12/13/the-knuth-morris-pratt-algorithm-in-my-own-words/
#include "StringSearcher_KMP.hpp"

IMonitoredStringSearcher*
StringSearcher_KMP::Factory::create()
{
    return new StringSearcher_KMP();
}

StringSearcher_KMP::StringSearcher_KMP()
{
#if defined(MONITOR)
    this->from_indices_.reserve( 1000 );
    this->compare_word_indices_.reserve( 1000 );
    this->compare_from_indices_.reserve( 1000 );
#endif
}

std::size_t
StringSearcher_KMP::search( const std::string& word, const std::string& fromText ) const
{
#if defined(MONITOR)
    this->from_indices_.clear();
    this->compare_word_indices_.clear();
    this->compare_from_indices_.clear();
#endif
    if ( !word.empty() && word.length() <= fromText.length() )
    {
        // KMP: analyse word to find reusable patterns
        auto table = partial_match_table( word );

        std::size_t m = 0;
        std::size_t i = 0;
        while ( m <= fromText.length() - word.length() )
        {
#if defined(MONITOR)
            // Monitor how index m jump
            this->from_indices_.push_back( m );
#endif
            while ( i < word.length() )
            {
#if defined(MONITOR)
                // Monitor the pair of characters begin compared, also counter the number of comparision
                this->compare_word_indices_.push_back( i );
                this->compare_from_indices_.push_back( m + i );
#endif
                if ( word[ i ] != fromText[ m + i ] )
                {
                    if ( i != 0 )
                    {
                        std::size_t reusable_size = table[ i - 1 ];

                        // KMP: determine the chars to be skipped
                        {
                            std::size_t skip = i - reusable_size;
                            m += skip;
                        }
                        i = 0; // rewind to test from beginning of word

                        // KMP: re-use the partially matched pattern
                        i += reusable_size;
                        // No need to advance m here, as m + i is used.
                    }
                    else
                    {
                        // move forward fromText
                        ++m;
                    }

                    break;
                }
                else
                {
                    ++i;
                }
            }

            if ( i == word.length() )
                return m;
        }
    }

    return npos;
}


//            * * *        * * *
// +-+-+-+-+-+-+-+-+  +-+-+-+-+-+
// |A|B|A|B|C|A|B|A|  |A|B|A|B|A|
// +-+-+-+-+-+-+-+-+  +-+-+-+-+-+
//  * * *              * * *

// TODO: Optimize the table generation
std::vector<std::size_t>
StringSearcher_KMP::partial_match_table( const std::string& word )
{
    std::vector<std::size_t> table( word.length() );

    // Fill each slot of the table
    for ( std::size_t i = 0; i < table.size(); ++i )
    {
        // Try to find the largest overlapped proper-prefix and proper-suffix
        for ( std::size_t prefix_suffix_length = i; prefix_suffix_length >= 1; --prefix_suffix_length )
        {
            bool found = true;
            for ( std::size_t k = 0; k < prefix_suffix_length; ++k )
            {
                // compare prefix at k and suffix at k
                if ( word[ k ] != word[ i - prefix_suffix_length + 1 + k ] )
                {
                    found = false;
                    break;
                }
            }

            if ( found )
            {
                table[ i ] = prefix_suffix_length;
                break;
            }
        }
    }

    return table;
}
