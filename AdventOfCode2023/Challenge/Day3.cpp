#include "Utilities.h"

namespace Day3
{
    std::vector<std::string> loadInput( std::istream& stream )
    {
        std::vector<std::string> data;
        for( std::string line; std::getline( stream, line ); )
            data.push_back( std::move( line ) );
        return data;
    }

    struct Vec2
    {
        int64_t x = 0;
        int64_t y = 0;
        auto operator<=>( const Vec2& ) const = default;
    };

    std::map<Vec2, char> getSymbolPositions( const std::vector<std::string>& data )
    {
        std::map<Vec2, char> symbols;
        for( int64_t y = 0; y < data.size(); y++ )
        {
            for( int64_t x = 0; x < data[ y ].size(); x++ )
            {
                char c = data[ y ][ x ];
                if( !std::isdigit( c ) && c != '.' )
                    symbols.emplace( Vec2{ x,y }, c );
            }
        }
        return symbols;
    }

    bool isNumberNearSymbol( const Vec2& position, int64_t length, const std::map<Vec2, char>& symbols )
    {
        if( symbols.count( Vec2{ position.x - 1, position.y } ) || symbols.count( Vec2{ position.x + length, position.y } ) )
            return true;
        for( auto y : { position.y - 1, position.y + 1 } )
        {
            for( int64_t x = position.x - 1; x < position.x + length + 1; x++ )
            {
                if( symbols.count( Vec2{ x, y } ) )
                    return true;
            }
        }
        return false;
    }

    void forEachNumber( const std::vector<std::string>& data, auto callback )
    {
        for( int64_t y = 0; y < data.size(); y++ )
        {
            auto searchStart = data[ y ].begin();
            auto findStart = [ & ] {
                return std::find_if( searchStart, data[ y ].end(), [] ( char c ) { return std::isdigit( c ); } );
                };

            for( auto startNumber = findStart(); startNumber != data[ y ].end(); startNumber = findStart() )
            {
                auto endNumber = std::find_if( startNumber, data[ y ].end(), [] ( char c ) { return !std::isdigit( c ); } );
                auto value = std::stoll( std::string( startNumber, endNumber ) );
                auto position = Vec2{ std::distance( data[ y ].begin(), startNumber ), y };
                auto length = endNumber - startNumber;
                callback( value, position, length );
                searchStart = endNumber;
            }
        }
    }

    int64_t getSumOfParts( const std::vector<std::string>& data, const std::map<Vec2, char>& symbols )
    {
        int64_t sumParts = 0;
        forEachNumber( data, [&] ( auto value, auto position, auto length ) {
            if( isNumberNearSymbol( position, length, symbols ) )
                sumParts += value;
            } );
        return sumParts;
    }

    void forEachTestPosition( const Vec2& position, int64_t length, auto callback )
    {
        callback( Vec2{ position.x - 1, position.y } );
        callback( Vec2{ position.x + length, position.y } );
        for( auto y : { position.y - 1, position.y + 1 } )
        {
            for( int64_t x = position.x - 1; x < position.x + length + 1; x++ )
                callback( Vec2{ x,y } );
        }
    }

    void addGears( std::map<Vec2, std::vector<int64_t>>& gears, const Vec2& position, int64_t length, int64_t partValue, const std::map<Vec2, char>& symbols )
    {
        forEachTestPosition( position, length, [ & ] ( const auto& testPosition ) {
            if( auto symbol = symbols.find( testPosition ); symbol != symbols.end() && symbol->second == '*' )
                gears[ testPosition ].push_back( partValue );
        } );
    }

    std::map<Vec2, std::vector<int64_t>> getGears( const std::vector<std::string>& data, const std::map<Vec2, char>& symbols )
    {
        std::map<Vec2, std::vector<int64_t>> gears;
        forEachNumber( data, [ & ] ( auto value, auto position, auto length ) {
            addGears( gears, position, length, value, symbols );
            } );
        std::erase_if( gears, [] ( auto& gear ) { return gear.second.size() != 2; } );
        return gears;
    }

    int64_t getSumGearRatios( const std::vector<std::string>& data, const std::map<Vec2, char>& symbols )
    {
        auto gears = getGears( data, symbols );
        return std::accumulate( gears.begin(), gears.end(), 0ll, [] ( int64_t sum, auto& gear ) { return sum + gear.second[ 0 ] * gear.second[ 1 ]; } );
    }
}

void executeDay3()
{
    std::ifstream file( "input/Day3.txt" );
    auto data = Day3::loadInput( file );
    auto symbols = Day3::getSymbolPositions( data );
    fmt::print( "Sum of valid parts: {}\n", Day3::getSumOfParts( data, symbols ) );
    fmt::print( "Sum of gear ratios: {}\n", Day3::getSumGearRatios( data, symbols ) );
}
