#include "Utilities.h"

namespace Day4
{
    struct Card
    {
        int64_t id = 0;
        std::set<int64_t> winningNumbers;
        std::set<int64_t> gameNumbers;
    };

    auto toNumbersSet( const auto& match )
    {
        return std::string_view{ match.first, match.second }
            | std::views::split( ' ' )
            | std::views::filter( [] ( auto range ) { return !std::ranges::empty( range ); } )
            | std::views::transform( [] ( auto range ) { return std::stoll( std::string{ range.begin(), range.end() } ); } )
            | std::ranges::to<std::set>();
    }

    Card parseCard( const std::string& line )
    {
        static std::regex regex( R"(Card\s+(\d+): (.*) \| (.*))" );
        std::smatch match;
        if( !std::regex_match( line, match, regex ) )
            throw std::runtime_error( "invalid input" );

        return { std::stoll( match[ 1 ] ), toNumbersSet( match[ 2 ] ), toNumbersSet( match[ 3 ] ) };
    }

    std::vector<Card> parseInput( std::istream& file )
    {
        std::vector<Card> cards;

        for( std::string line; std::getline( file, line ); )
            cards.push_back( parseCard( line ) );

        return cards;
    }

    int64_t getNumMatchingNumbers( const Card& card )
    {
        std::vector<int64_t> winningNumbers;
        std::ranges::set_intersection( card.winningNumbers, card.gameNumbers, back_inserter( winningNumbers ) );
        return winningNumbers.size();
    }
    int64_t getCardPoints( const Card& card )
    {
        const auto numWinningNumbers = getNumMatchingNumbers( card );
        if( numWinningNumbers == 0 )
            return 0;
        return 1ll << ( numWinningNumbers - 1ll );
    }

    int64_t getTotalPoints( const std::vector<Card>& cards )
    {
        return std::accumulate( cards.begin(), cards.end(), 0ll, [] ( int64_t sum, auto& card ) { return sum + getCardPoints( card ); } );
    }

    void handleCard( const Card& card, std::vector<int64_t>& numberOfCards )
    {
        const auto numWinningNumbers = getNumMatchingNumbers( card );
        for( int64_t cardId = card.id; cardId < card.id + numWinningNumbers; cardId++ )
            numberOfCards[ cardId ]+= numberOfCards[card.id-1];
    }

    int64_t getTotalNumberOfCards( const std::vector<Card>& cards )
    {
        std::vector<int64_t> numberOfCards( cards.size(), 1ll );
        for( auto& card : cards )
            handleCard( card, numberOfCards );
         return std::accumulate( numberOfCards.begin(), numberOfCards.end(), 0ll);
    }
}

void executeDay4()
{
    std::ifstream file( "input/Day4.txt" );
    auto cards = Day4::parseInput( file );
    fmt::print( "Total points: {}\n", Day4::getTotalPoints( cards ) );
    fmt::print( "Total number of cards: {}\n", Day4::getTotalNumberOfCards( cards ) );
}
