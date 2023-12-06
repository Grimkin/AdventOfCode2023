#include "Utilities.h"

namespace Day6
{
    struct BoatRace
    {
        int64_t time = 0;
        int64_t distance = 0;
    };

    auto toNumbers()
    {
        return std::views::drop_while( [] ( char c ) { return !std::isdigit( c ); } )
            | std::views::split( ' ' )
            | std::views::filter( [] ( auto range ) { return !std::ranges::empty( range ); } )
            | std::views::transform( [] ( auto range ) { return std::stoll( std::string( range.begin(), range.end() ) ); } );
    }

    std::vector<BoatRace> parseInput( std::istream& stream )
    {
        std::string timeLine;
        std::getline( stream, timeLine );
        std::string distanceLine;
        std::getline( stream, distanceLine );

        return std::views::zip( timeLine | toNumbers(), distanceLine | toNumbers() )
            | std::views::transform( [] ( auto tuple ) { return BoatRace{ std::get<0>( tuple ), std::get<1>( tuple ) }; } )
            | std::ranges::to<std::vector>();
    }

    int64_t getTotalPoints( const std::vector<BoatRace>& boatRaces )
    {
        return std::accumulate( boatRaces.begin(), boatRaces.end(), 1ll, [] ( int64_t result, auto& boatRace ) { return result * getBoatRacePoints( boatRace ); } );
    }

    auto getNumber( const std::string& line )
    {
        auto result = line
            | std::views::drop_while( [] ( char c ) { return !std::isdigit( c ); } )
            | std::views::filter( [] ( auto c ) { return c != ' '; } );

        return std::stoll( std::string( result.begin(), result.end() ) );
    }

    BoatRace parseInput2( std::istream& stream )
    {
        std::string timeLine;
        std::getline( stream, timeLine );
        std::string distanceLine;
        std::getline( stream, distanceLine );

        return { getNumber( timeLine ), getNumber( distanceLine ) };
    }

    int64_t getBoatRacePoints( const BoatRace& boatRace )
    {
        auto minTime = std::ceil( boatRace.time / 2. - sqrt( ( boatRace.time * boatRace.time / 4. ) - boatRace.distance ) );
        auto maxTime = std::floor( boatRace.time / 2. + sqrt( ( boatRace.time * boatRace.time / 4. ) - boatRace.distance ) );

        return maxTime - minTime + 1;
    }
}

void executeDay6()
{
    std::ifstream file( "input/Day6.txt" );
    auto boatRaces = Day6::parseInput( file );
    fmt::print( "Get total points: {}\n", Day6::getTotalPoints( boatRaces ) );
    file.seekg( 0 );
    fmt::print( "Get actual points: {}\n", Day6::getBoatRacePoints( Day6::parseInput2( file ) ) );
}
