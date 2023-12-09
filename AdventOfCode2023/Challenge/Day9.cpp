#include "Utilities.h"

namespace Day9
{
    using History = std::vector<int64_t>;
    std::vector<History> parseInput( std::istream& stream )
    {
        std::vector<History> histories;
        for( std::string line; std::getline( stream, line ); )
            histories.push_back( line
                | std::views::split( ' ' )
                | std::views::transform( [] ( auto range ) { return std::stoll( std::string( range.begin(), range.end() ) ); } )
                | std::ranges::to<std::vector>() );
        return histories;
    }

    int64_t getExtrapolatedValue( const History& history )
    {
        std::vector<History> extrapolations{ history };

        while( !std::ranges::all_of( extrapolations.back(), [] ( auto v ) { return v == 0; } ) )
        {
            extrapolations.push_back( extrapolations.back()
                | std::views::adjacent<2>
                | std::views::transform( [] ( auto tuple ) { return std::get<1>( tuple ) - std::get<0>( tuple ); } )
                | std::ranges::to<std::vector>() );
        }

        auto lastNumbers = extrapolations | std::views::transform( [] ( auto range ) { return range.back(); } );
        return std::accumulate( lastNumbers.begin(), lastNumbers.end(), 0ll );
    }

    int64_t getSumOfExtropolatedHistories( const std::vector<History>& histories )
    {
        return std::accumulate( histories.begin(), histories.end(), 0ll, [] ( auto sum, auto history ) { return sum + getExtrapolatedValue( history ); } );
    }

    int64_t getNegativeExtrapolatedValue( const History& history )
    {
        std::vector<History> extrapolations{ history };

        while( !std::ranges::all_of( extrapolations.back(), [] ( auto v ) { return v == 0; } ) )
        {
            extrapolations.push_back( extrapolations.back()
                | std::views::adjacent<2>
                | std::views::transform( [] ( auto tuple ) { return std::get<1>( tuple ) - std::get<0>( tuple ); } )
                | std::ranges::to<std::vector>() );
        }

        int64_t extrapolatedValue = 0;

        for( auto& extrapolation : extrapolations | std::views::reverse )
            extrapolatedValue = extrapolation.front() - extrapolatedValue;

        return extrapolatedValue;
    }

    int64_t getSumOfNegativeExtropolatedHistories( const std::vector<History>& histories )
    {
        return std::accumulate( histories.begin(), histories.end(), 0ll, [] ( auto sum, auto history ) { return sum + getNegativeExtrapolatedValue( history ); } );
    }
}

void executeDay9()
{
    std::ifstream file( "input/Day9.txt" );
    auto histories = Day9::parseInput( file );
    fmt::print( "Sum of extrapolated history values: {}\n", Day9::getSumOfExtropolatedHistories( histories ) );
    fmt::print( "Sum of negative extrapolated history values: {}\n", Day9::getSumOfNegativeExtropolatedHistories( histories ) );
}
