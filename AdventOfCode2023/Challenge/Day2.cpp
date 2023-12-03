#include "Utilities.h"

namespace Day2
{
    struct GamePass
    {
        int64_t numRedCubes = 0;
        int64_t numGreenCubes = 0;
        int64_t numBlueCubes = 0;
    };
    struct Game
    {
        int64_t id = 0;
        std::vector<GamePass> passes;
    };

    GamePass parseGamePass( auto gamePassString )
    {
        GamePass gamePass;
        for( auto game : gamePassString | std::views::split( ',' ) )
        {
            auto cubes = std::string_view{ game } | std::views::split( ' ' ) | std::views::drop_while( [] ( auto view ) { return view.begin() == view.end(); } );
            auto numCubes = std::stoll( std::string( std::string_view{ *cubes.begin() } ) );
            auto cubeColor = std::string_view{ *std::next( cubes.begin() ) };
            if( cubeColor == "red" )
                gamePass.numRedCubes = numCubes;
            else if( cubeColor == "green" )
                gamePass.numGreenCubes = numCubes;
            else if( cubeColor == "blue" )
                gamePass.numBlueCubes = numCubes;
        }
        return gamePass;
    }

    Game parseGame( const std::string& line )
    {
        static std::regex gameIdRegex( R"(Game (\d+): (.*))" );
        std::smatch match;
        if( !std::regex_match( line, match, gameIdRegex ) )
            throw std::runtime_error( "invalide line" );

        Game game{ std::stoll( match[ 1 ] ) };

        auto games = match[ 2 ].str();
        for( auto gamePass : games | std::views::split( ';' ) )
            game.passes.push_back( parseGamePass( gamePass ) );

        return game;
    }

    std::vector<Game> parseInput( std::istream& stream )
    {
        std::vector<Game> games;
        for( std::string line; std::getline( stream, line ); )
            games.push_back( parseGame( line ) );
        return games;
    }

    bool isGamePassValid( const GamePass& gamePass )
    {
        constexpr int64_t numRedCubes = 12, numGreenCubes = 13, numBlueCubes = 14;

        return gamePass.numRedCubes <= numRedCubes && gamePass.numGreenCubes <= numGreenCubes && gamePass.numBlueCubes <= numBlueCubes;
    }

    bool isGameValid( const Game& game )
    {
        return std::ranges::all_of( game.passes, &isGamePassValid );
    }

    int64_t getSumOfValidGames( const std::vector<Game>& games )
    {
        return std::accumulate( games.begin(), games.end(), 0ll, [] ( int64_t sum, const Game& game ) { return isGameValid( game ) ? sum + game.id : sum; } );
    }

    int64_t getPowerOfGame( const Game& game )
    {
        int64_t minRedCubes = 0, minGreenCubes = 0, minBlueCubes = 0;

        for( auto& gamePass : game.passes )
        {
            minRedCubes = std::max( minRedCubes, gamePass.numRedCubes );
            minGreenCubes = std::max( minGreenCubes, gamePass.numGreenCubes );
            minBlueCubes = std::max( minBlueCubes, gamePass.numBlueCubes );
        }

        return minRedCubes * minGreenCubes * minBlueCubes;
    }

    int64_t getSumOfPowerOfGames( const std::vector<Game>& games )
    {
        return std::accumulate( games.begin(), games.end(), 0ll, [] ( int64_t sum, const Game& game ) { return getPowerOfGame( game ) + sum; } );
    }
}

void executeDay2()
{
    std::ifstream file( "input/Day2.txt" );
    auto games = Day2::parseInput( file );
    fmt::print( "Sum of valid game ids: {}\n", getSumOfValidGames( games ) );
    fmt::print( "Sum of power of games: {}\n", getSumOfPowerOfGames( games ) );
}
