#include "Utilities.h"

namespace Day10
{
    enum class ElementType
    {
        Start,
        Vertical,
        Horizontal,
        NE_Bend,
        NW_Bend,
        SW_Bend,
        SE_Bend,
        Empty
    };

    using Map = std::vector<std::vector<ElementType>>;

    ElementType toElementType( char c )
    {
        switch( c )
        {
            case 'S':
                return ElementType::Start;
            case '|':
                return ElementType::Vertical;
            case '-':
                return ElementType::Horizontal;
            case 'L':
                return ElementType::NE_Bend;
            case 'J':
                return ElementType::NW_Bend;
            case 'F':
                return ElementType::SE_Bend;
            case '7':
                return ElementType::SW_Bend;
            case '.':
                return ElementType::Empty;
        }
        throw std::runtime_error( "invalid element type" );
    }

    Map parseInput( std::istream& stream )
    {
        Map map;
        for( std::string line; std::getline( stream, line ); )
            map.push_back( line | std::views::transform( &toElementType ) | std::ranges::to<std::vector>() );
        return map;
    }

    struct Vec2
    {
        int64_t x = 0;
        int64_t y = 0;

        auto operator<=>( const Vec2& rhs ) const = default;
    };

    Vec2 findStartPosition( const Map& map )
    {
        for( auto [y, row] : map | std::views::enumerate )
        {
            if( auto start = std::ranges::find( row, ElementType::Start ); start != row.end() )
                return { std::distance( row.begin(), start ), y };
        }
        throw std::runtime_error( "no start" );
    }

    enum class Direction
    {
        North,
        South,
        East,
        West
    };

    std::pair<Vec2, Direction> getStep( const Vec2& position, Direction direction )
    {
        switch( direction )
        {
            case Direction::North:
                return { { position.x, position.y - 1 }, Direction::North };
            case Direction::South:
                return { { position.x, position.y + 1 }, Direction::South };
            case Direction::East:
                return { { position.x + 1, position.y }, Direction::East };
            case Direction::West:
                return { { position.x - 1, position.y }, Direction::West };
        }
        throw std::runtime_error( "invalid direction" );
    }

    std::pair<Vec2,Direction> getValidFirstStep( const Map& map, const Vec2& start )
    {
        if( auto top = map[ start.y - 1 ][ start.x ]; top == ElementType::Vertical || top == ElementType::SE_Bend || top == ElementType::SW_Bend )
            return getStep( start, Direction::North );
        if( auto bot = map[ start.y + 1 ][ start.x ]; bot == ElementType::Vertical || bot == ElementType::NE_Bend || bot == ElementType::NW_Bend )
            return getStep( start, Direction::South );
        if( auto left = map[ start.y ][ start.x - 1 ]; left == ElementType::Horizontal || left == ElementType::SE_Bend || left == ElementType::NE_Bend )
            return getStep( start, Direction::West );
        if( auto right = map[ start.y ][ start.x + 1 ]; right == ElementType::Horizontal || right == ElementType::SW_Bend || right == ElementType::NW_Bend )
            return getStep( start, Direction::East );

        throw std::runtime_error( "no valid start step" );
    }

    std::pair<Vec2, Direction> getNextStep( const Map& map, const std::pair<Vec2, Direction>& current )
    {
        auto& [position, direction] = current;
        switch( map[ position.y ][ position.x ] )
        {
            case Day10::ElementType::Vertical:
            case Day10::ElementType::Horizontal:
                return getStep( position, direction );
            case Day10::ElementType::NE_Bend:
                if( direction == Direction::South )
                    return getStep( position, Direction::East );
                else
                    return getStep( position, Direction::North );
            case Day10::ElementType::NW_Bend:
                if( direction == Direction::South )
                    return getStep( position, Direction::West );
                else
                    return getStep( position, Direction::North );
            case Day10::ElementType::SW_Bend:
                if( direction == Direction::North )
                    return getStep( position, Direction::West );
                else
                    return getStep( position, Direction::South );
            case Day10::ElementType::SE_Bend:
                if( direction == Direction::North )
                    return getStep( position, Direction::East );
                else
                    return getStep( position, Direction::South );
        }
        throw std::runtime_error( "invalid element" );
    }

    int64_t getLoopLength( const Map& map )
    {
        const auto start = findStartPosition( map );
        auto currentPosition = getValidFirstStep( map, start );
        int64_t numberOfSteps = 1;
        for( ;currentPosition.first != start; numberOfSteps++ )
            currentPosition = getNextStep( map, currentPosition );
        return numberOfSteps;
    }

    std::map<Vec2,ElementType> getLoopPositions( const Map& map )
    {
        std::map<Vec2, ElementType> loopPositions;
        const auto start = findStartPosition( map );
        auto currentPosition = getValidFirstStep( map, start );
        loopPositions.emplace( start, ElementType::Start );
        while( currentPosition.first != start )
        {
            loopPositions.emplace( currentPosition.first, map[ currentPosition.first.y ][ currentPosition.first.x ] );
            currentPosition = getNextStep( map, currentPosition );
        }
        return loopPositions;
    }

    using FillMap = std::vector<std::vector<bool>>;

    void addElementToFillMap( FillMap& map, const Vec2& position, ElementType type )
    {
        switch( type )
        {
            case Day10::ElementType::Start:
                for( size_t y = position.y * 3; y < position.y * 3 + 3; y++ )
                    for( size_t x = position.x * 3; x < position.x * 3 + 3; x++ )
                        map[ y ][ x ] = true;
                break;
            case Day10::ElementType::Vertical:
                for( size_t y = position.y * 3; y < position.y * 3 + 3; y++ )
                    map[ y ][ position.x * 3 + 1 ] = true;
                break;
            case Day10::ElementType::Horizontal:
                for( size_t x = position.x * 3; x < position.x * 3 + 3; x++ )
                    map[ position.y * 3 + 1 ][ x ] = true;
                break;
            case Day10::ElementType::NE_Bend:
                map[ position.y * 3 + 0 ][ position.x * 3 + 1 ] = true;
                map[ position.y * 3 + 1 ][ position.x * 3 + 1 ] = true;
                map[ position.y * 3 + 1 ][ position.x * 3 + 2 ] = true;
                break;
            case Day10::ElementType::NW_Bend:
                map[ position.y * 3 + 0 ][ position.x * 3 + 1 ] = true;
                map[ position.y * 3 + 1 ][ position.x * 3 + 0 ] = true;
                map[ position.y * 3 + 1 ][ position.x * 3 + 1 ] = true;
                break;
            case Day10::ElementType::SW_Bend:
                map[ position.y * 3 + 1 ][ position.x * 3 + 0 ] = true;
                map[ position.y * 3 + 1 ][ position.x * 3 + 1 ] = true;
                map[ position.y * 3 + 2 ][ position.x * 3 + 1 ] = true;
                break;
            case Day10::ElementType::SE_Bend:
                map[ position.y * 3 + 1 ][ position.x * 3 + 1 ] = true;
                map[ position.y * 3 + 1 ][ position.x * 3 + 2 ] = true;
                map[ position.y * 3 + 2 ][ position.x * 3 + 1 ] = true;
                break;
        }
    }

    FillMap getFillMap( const Map& map )
    {
        FillMap fillMap( map.size() * 3 );
        const auto rowLength = map.front().size() * 3;
        for( auto& row : fillMap )
            row.resize( rowLength );

        for( auto& [position, type] : getLoopPositions( map ) )
            addElementToFillMap( fillMap, position, type );

        return fillMap;
    }

    void floodFill( FillMap& map )
    {
        std::vector<Vec2> positions{ Vec2{ 0,0 } };
        while( !positions.empty() )
        {
            auto position = positions.back();
            positions.pop_back();
            if( map[ position.y ][ position.x ] )
                continue;

            map[ position.y ][ position.x ] = true;
            if( position.x > 0  )
                positions.push_back( { position.x - 1, position.y } );
            if( position.x < map.front().size() - 1 )
                positions.push_back( { position.x + 1, position.y } );
            if( position.y > 0 )
                positions.push_back( { position.x, position.y - 1 } );
            if( position.y < map.size() - 1 )
                positions.push_back( { position.x, position.y + 1 } );
        }
    }

    bool isFilled( const FillMap& map, const Vec2& position )
    {
        for( size_t y = 0; y < 3; y++ )
        {
            for( size_t x = 0; x < 3; x++ )
                if( map[ position.y + y ][ position.x + x ] )
                    return true;
        }
        return false;
    }

    int64_t getNumFilled( const FillMap& map )
    {
        int64_t numFilled = 0;
        for( int64_t y = 0; y < ssize( map ); y+=3 )
            for( int64_t x = 0; x < ssize( map[ y ] ); x += 3 )
                numFilled += isFilled( map, { x, y } );
        return numFilled;
    }

    int64_t getNumberEnclosed( const Map& map )
    {
        auto fillMap = getFillMap( map );
        floodFill( fillMap );

        return map.size() * map.front().size() - getNumFilled( fillMap );
    }
}

void executeDay10()
{
    std::ifstream file( "input/Day10.txt" );
    auto map = Day10::parseInput( file );
    fmt::print( "Intersection distance: {}\n", Day10::getLoopLength( map ) / 2 );
    fmt::print( "Number of enclosed cells: {}\n", Day10::getNumberEnclosed( map ) );
}
