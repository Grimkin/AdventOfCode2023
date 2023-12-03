#include "Utilities.h"

namespace Day1
{
    std::vector<std::string> loadInput( std::istream& stream )
    {
        std::vector<std::string> data;
        for( std::string line; std::getline( stream, line ); )
            data.push_back( line );
        return data;
    }
    int64_t toDigit( char c )
    {
        return c - '0';
    }

    int64_t getCalibrationValue( const std::string& line )
    {
        const auto firstDigit = std::ranges::find_if( line, [] ( char c )-> bool { return std::isdigit( c ); } );
        const auto reversedLine = line | std::views::reverse;
        const auto lastDigit = std::ranges::find_if( line | std::views::reverse, [] ( char c )-> bool { return std::isdigit( c ); } );

        if( firstDigit == line.end() || lastDigit == reversedLine.end() )
            throw std::runtime_error( "invalid line" );

        return toDigit( *firstDigit ) * 10 + toDigit( *lastDigit );
    }

    std::pair<std::pair<size_t, int64_t>, std::pair<size_t, int64_t>> findWrittenNumber( std::string line )
    {
        using AlphaDigit = std::pair<std::string_view, int64_t>;
        constexpr std::array<AlphaDigit, 9> digitWords = { AlphaDigit{"one", 1},  AlphaDigit{"two", 2},  AlphaDigit{"three", 3},  AlphaDigit{"four", 4},  AlphaDigit{"five", 5},  AlphaDigit{"six", 6},  AlphaDigit{"seven", 7},  AlphaDigit{"eight", 8},  AlphaDigit{"nine", 9} };
        std::pair<size_t, int64_t> minAlphaDigit{ std::string::npos, 0 };
        std::pair<size_t, int64_t> maxAlphaDigit{ 0, 0 };
        for( auto& [name, value] : digitWords )
        {
            auto first = line.find( name );
            auto last = line.rfind( name );
            if( first == std::string::npos )
                continue;
            if( first < minAlphaDigit.first )
                minAlphaDigit = { first, value };
            if( last > maxAlphaDigit.first )
                maxAlphaDigit = { last, value };
        }
        return { minAlphaDigit, maxAlphaDigit };
    }

    int64_t getCalibrationValue2( const std::string& line )
    {
        const auto firstDigit = std::ranges::find_if( line, [] ( char c )-> bool { return std::isdigit( c ); } );
        const auto reversedLine = line | std::views::reverse;
        const auto lastDigit = std::ranges::find_if( reversedLine, [] ( char c )-> bool { return std::isdigit( c ); } );
        const auto [firstAlphaDigit, lastAlphaDigit] = findWrittenNumber( line );

        int64_t first = 0;
        if( firstDigit != line.end() && std::distance( line.begin(), firstDigit ) < firstAlphaDigit.first )
            first = toDigit( *firstDigit );
        else
            first = firstAlphaDigit.second;

        int64_t last = 0;
        if( lastDigit != reversedLine.end() && line.size() - std::distance( reversedLine.begin(), lastDigit ) > lastAlphaDigit.first )
            last = toDigit( *lastDigit );
        else
            last = lastAlphaDigit.second;

        return first * 10 + last;
    }

    int64_t getSumCalibrationValues2( const std::vector<std::string>& lines )
    {
        return std::accumulate( lines.begin(), lines.end(), 0ll, [] ( int64_t sum, auto& line ) { return sum + getCalibrationValue2( line ); } );
    }

    int64_t getSumCalibrationValues( const std::vector<std::string>& lines )
    {
        return std::accumulate( lines.begin(), lines.end(), 0ll, [] ( int64_t sum, auto& line ) { return sum + getCalibrationValue( line ); } );
    }
}

void executeDay1()
{
    std::ifstream input( "Input/Day1.txt" );
    auto lines = Day1::loadInput( input );
    fmt::print( "Day1: Sum of calibration values: {}\n", Day1::getSumCalibrationValues( lines ) );
    fmt::print( "Day1: Sum of calibration values 2: {}\n", Day1::getSumCalibrationValues2( lines ) );
}