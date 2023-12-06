#include "Utilities.h"

namespace Day5
{
    struct Range
    {
        int64_t start;
        int64_t length;
    };
    class RangeMap
    {
    public:
        struct SubMap
        {
            int64_t outputStart = 0;
            int64_t inputStart = 0;
            int64_t length = 0;
        };
        void addSubMap( const SubMap& subMap )
        {
            m_subMaps.push_back( subMap );
        }

        int64_t getMappedValue( int64_t value ) const
        {
            for( auto& [outputStart, inputStart, length] : m_subMaps )
            {
                if( value >= inputStart && value < inputStart + length )
                    return value - inputStart + outputStart;
            }
            return value;
        }


        std::vector<Range> getMappedRange( const Range& range ) const;
    private:
        std::vector<SubMap> m_subMaps;
    };

    struct Almanac
    {
        std::vector<int64_t> seeds;
        std::vector<RangeMap> maps;
    };

    std::vector<int64_t> parseSeeds( const std::string& line )
    {
        return line
            | std::views::drop_while( [] ( char c ) { return !std::isdigit( c ); } )
            | std::views::split( ' ' )
            | std::views::transform( [] ( auto range ) { return std::stoll( std::string( range.begin(), range.end() ) ); } )
            | std::ranges::to<std::vector>();
    }

    RangeMap::SubMap parseSubMap( const std::string& line )
    {
        auto parameters = line
            | std::views::split( ' ' )
            | std::views::transform( [] ( auto range ) { return std::stoll( std::string( range.begin(), range.end() ) ); } );
        return { *parameters.begin(), *std::next( parameters.begin(), 1 ), *std::next( parameters.begin(), 2 ) };
    }

    RangeMap parseMap( std::istream& stream )
    {
        RangeMap rangeMap;
        for( std::string line; std::getline( stream, line ); )
        {
            if( line.empty() )
                break;
            rangeMap.addSubMap( parseSubMap( line ) );
        }
        return rangeMap;
    }

    Almanac parseInput( std::istream& stream )
    {
        std::string line;
        std::getline( stream, line );
        Almanac almanac{ parseSeeds( line ) };
        std::getline( stream, line );

        for( std::string line; std::getline( stream, line ); )
            almanac.maps.push_back( parseMap( stream ) );
        return almanac;
    }

    int64_t getBestLocation( const Almanac& almanac )
    {
        std::vector<int64_t> mappedSeeds = almanac.seeds;
        for( auto& mappedSeed : mappedSeeds )
        {
            for( auto& map : almanac.maps )
                mappedSeed = map.getMappedValue( mappedSeed );
        }
        std::ranges::partial_sort( mappedSeeds, mappedSeeds.begin() + 1 );
        return mappedSeeds[ 0 ];
    }

    Range convertTopRange( std::vector<Range>& inputRanges, const std::vector<RangeMap::SubMap>& subMaps )
    {
        auto currentRange = inputRanges.back();
        inputRanges.pop_back();
        for( auto& [outputStart, inputStart, length] : subMaps )
        {
            if( currentRange.start >= inputStart )
            {
                if( currentRange.start >= inputStart + length )
                    continue;
                // complete inside
                if( currentRange.start + currentRange.length <= inputStart + length )
                    return { currentRange.start - inputStart + outputStart, currentRange.length };
                // end outside
                inputRanges.push_back( { inputStart + length, ( currentRange.start + currentRange.length ) - ( inputStart + length ) } );
                return { currentRange.start - inputStart + outputStart, inputStart + length - currentRange.start };
            }
            else if( currentRange.start + currentRange.length > inputStart )
            {
                inputRanges.push_back( { currentRange.start, inputStart - currentRange.start } );
                // end inside
                if( currentRange.start + currentRange.length <= inputStart + length )
                    return { outputStart, currentRange.start + currentRange.length - inputStart };
                // end outside
                inputRanges.push_back( { inputStart + length, ( currentRange.start + currentRange.length ) - ( inputStart + length ) } );
                return { outputStart, length };
            }
        }
        return currentRange;
    }

    std::vector<Range> RangeMap::getMappedRange( const Range& range ) const
    {
        std::vector<Range> inputRanges{ range };
        std::vector<Range> outputRanges;
        while( !inputRanges.empty() )
            outputRanges.push_back( convertTopRange( inputRanges, m_subMaps ) );
        return outputRanges;
    }

    int64_t getBestLocation2( const Almanac& almanac )
    {
        std::vector<std::vector<Range>> seedRanges = almanac.seeds
            | std::views::chunk( 2 )
            | std::views::transform( [] ( auto&& range ) { return std::vector<Range>{ Range{ *range.begin(), *std::next( range.begin() ) } }; } )
            | std::ranges::to<std::vector>();
        for( auto& seedRange : seedRanges )
        {
            for( auto& map : almanac.maps )
            {
                std::vector<Range> outRanges;
                for( auto& range : seedRange )
                    outRanges.append_range( map.getMappedRange( range ) );
                seedRange = outRanges;
            }
        }
        for( auto& range : seedRanges )
            std::ranges::partial_sort( range, range.begin() + 1, [] ( auto& lhs, auto& rhs ) { return lhs.start < rhs.start; } );
        std::ranges::partial_sort( seedRanges, seedRanges.begin() + 1, [] ( auto& lhs, auto& rhs ) { return lhs.front().start < rhs.front().start; } );
        return seedRanges.front().front().start;
    }
}

void executeDay5()
{
    std::ifstream file( "input/Day5.txt" );
    auto almanac = Day5::parseInput( file );
    fmt::print( "Best location: {}\n", Day5::getBestLocation( almanac ) );
    fmt::print( "Best location2: {}\n", Day5::getBestLocation2( almanac ) );
}
