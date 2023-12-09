#include "Utilities.h"

namespace Day7
{
    enum class Card
    {
        Ace,
        King,
        Queen,
        Jack,
        Ten,
        Nine,
        Eight,
        Seven,
        Six,
        Five,
        Four,
        Three,
        Two,
        Joker
    };
    
    auto operator<=>( const Card& lhs, const Card& rhs )
    {
        using T = std::underlying_type_t<Card>;
        return static_cast<T>( lhs ) <=> static_cast<T>( rhs );
    }

    enum class HandValue
    {
        Five,
        Four,
        Fullhouse,
        Three,
        TwoPair,
        Pair,
        HighCard
    };

    auto operator<=>( const HandValue& lhs, const HandValue& rhs )
    {
        using T = std::underlying_type_t<HandValue>;
        return static_cast<T>( lhs ) <=> static_cast<T>( rhs );
    }

    struct Hand
    {
        std::vector<Card> cards;
        int64_t bet;
        HandValue handValue;
        HandValue jokerHandValue;
        bool operator<( const Hand& rhs ) const
        {
            if( handValue == rhs.handValue )
                return cards < rhs.cards;
            return handValue < rhs.handValue;
        }
    };

    Card toCard( char c )
    {
        switch( c )
        {
            case 'A':
                return Card::Ace;
            case 'K':
                return Card::King;
            case 'Q':
                return Card::Queen;
            case 'J':
                return Card::Jack;
            case 'T':
                return Card::Ten;
            case '9':
                return Card::Nine;
            case '8':
                return Card::Eight;
            case '7':
                return Card::Seven;
            case '6':
                return Card::Six;
            case '5':
                return Card::Five;
            case '4':
                return Card::Four;
            case '3':
                return Card::Three;
            case '2':
                return Card::Two;
            default:
                throw std::runtime_error( "invalid card" );
        }
    }

    HandValue getHandValue( const std::array<int64_t, 5>& numCards )
    {
        if( numCards[ 4 ] == 1 )
            return HandValue::Five;
        if( numCards[ 3 ] == 1 )
            return HandValue::Four;
        if( numCards[ 2 ] == 1 && numCards[ 1 ] == 1 )
            return HandValue::Fullhouse;
        if( numCards[ 1 ] == 2 )
            return HandValue::TwoPair;
        if( numCards[ 2 ] == 1 )
            return HandValue::Three;
        if( numCards[ 1 ] == 1 )
            return HandValue::Pair;
        return HandValue::HighCard;
    }

    HandValue getHandValue( const std::vector<Card>& cards )
    {
        std::multiset<Card> sortedCards = cards | std::ranges::to<std::multiset>();
        std::array<int64_t, 5> numCards{ 0 };
        for( auto it = sortedCards.begin(); it != sortedCards.end(); )
        {
            auto count = sortedCards.count( *it );
            numCards[ count - 1 ]++;
            std::advance( it, count );
        }
        return getHandValue( numCards );
    }

    HandValue getJokerHandValue( const std::vector<Card>& cards )
    {
        std::multiset<Card> sortedCards = cards | std::ranges::to<std::multiset>();
        std::array<int64_t, 5> numCards{ 0 };
        int64_t numJokers = 0;
        for( auto it = sortedCards.begin(); it != sortedCards.end(); )
        {
            auto count = sortedCards.count( *it );
            if( *it == Card::Jack )
                numJokers += count;
            else
                numCards[ count - 1 ]++;
            std::advance( it, count );
        }
        if( numJokers == 5 )
            return HandValue::Five;
        auto handValue = getHandValue( numCards );
        switch( handValue )
        {
            case HandValue::Five:
                return HandValue::Five;
            case HandValue::Four:
                if( numJokers == 1 )
                    return HandValue::Five;
                return HandValue::Four;
            case HandValue::Fullhouse:
                return HandValue::Fullhouse;
            case HandValue::Three:
                if( numJokers == 1 )
                    return HandValue::Four;
                if( numJokers == 2 )
                    return HandValue::Five;
                return HandValue::Three;
            case HandValue::TwoPair:
                if( numJokers == 1 )
                    return HandValue::Fullhouse;
                return HandValue::TwoPair;
            case HandValue::Pair:
                if( numJokers == 1 )
                    return HandValue::Three;
                if( numJokers == 2 )
                    return HandValue::Four;
                if( numJokers == 3 )
                    return HandValue::Five;
                return HandValue::Pair;
            case HandValue::HighCard:
                if( numJokers == 1 )
                    return HandValue::Pair;
                if( numJokers == 2 )
                    return HandValue::Three;
                if( numJokers == 3 )
                    return HandValue::Four;
                if( numJokers == 4 )
                    return HandValue::Five;
        }
        return HandValue::HighCard;
    }

    Hand parseHand( const std::string& line )
    {
        auto betString = line
            | std::views::drop_while( [] ( auto c ) {return c != ' '; } );

        int64_t bet = std::stoll( std::string( betString.begin(), betString.end() ) );
        auto cards = line
            | std::views::take( 5 )
            | std::views::transform( toCard )
            | std::ranges::to<std::vector>();

        return { cards, bet, getHandValue( cards ), getJokerHandValue( cards ) };
    }

    std::vector<Hand> parseInput( std::istream& stream )
    {
        std::vector<Hand> hands;
        for( std::string line; std::getline( stream, line ); )
            hands.push_back( parseHand( line ) );
        return hands;
    }

    int64_t getTotalWinnings( std::vector<Hand> hands )
    {
        std::sort( hands.begin(), hands.end() );
        auto points = hands
            | std::views::reverse
            | std::views::enumerate
            | std::views::transform( [] ( auto tuple ) { return ( std::get<0>( tuple ) + 1 ) * std::get<1>( tuple ).bet; } )
            | std::ranges::to<std::vector>();

        return std::accumulate( points.begin(), points.end(), 0ll );
    }

    int64_t getTotalJokerWinnings( std::vector<Hand> hands )
    {
        for( auto& hand : hands )
        {
            std::ranges::replace( hand.cards, Card::Jack, Card::Joker );
            hand.handValue = hand.jokerHandValue;
        }

        return getTotalWinnings( std::move( hands ) );
    }
}

void executeDay7()
{
    std::ifstream file( "input/day7.txt" );
    auto hands = Day7::parseInput( file );
    fmt::print( "Total winning points: {}\n", Day7::getTotalWinnings( hands ) );
    fmt::print( "Total winning points with joker: {}\n", Day7::getTotalJokerWinnings( hands ) );
}
