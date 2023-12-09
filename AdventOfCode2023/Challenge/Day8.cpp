#include "Utilities.h"

namespace Day8
{
    using NodeId = std::array<char, 3>;
    using NodeMap = std::map<NodeId, std::pair<NodeId, NodeId>>;
    struct MapData
    {
        std::string  instructions;
        NodeMap nodes;
    };

    NodeId toNodeId( const std::string& node )
    {
        return { node[ 0 ], node[ 1 ], node[ 2 ] };
    }

    void addNode( NodeMap& nodeMap, const std::string& line )
    {
        static std::regex regex( R"((\w\w\w) = \((\w\w\w), (\w\w\w)\))" );
        std::smatch match;
        if( !std::regex_match( line, match, regex ) )
            throw std::runtime_error( "invalid node" );
        nodeMap.emplace( toNodeId( match[ 1 ] ), std::pair{ toNodeId( match[ 2 ] ), toNodeId( match[ 3 ] ) } );
    }

    MapData parseInput( std::istream& stream )
    {
        MapData mapData;
        std::getline( stream, mapData.instructions );

        std::string line;
        std::getline( stream, line );
        for( ; std::getline( stream, line ); )
            addNode( mapData.nodes, line );
        return mapData;
    }

    int64_t getMinNumberOfSteps( const MapData& mapData )
    {
        auto currentNode = mapData.nodes.find( NodeId{ 'A', 'A', 'A' } );
        auto currentInstruction = mapData.instructions.begin();
        int64_t numberOfSteps = 0;
        while( currentNode->first != NodeId{ 'Z', 'Z','Z' } )
        {
            numberOfSteps++;
            if( *currentInstruction == 'L' )
                currentNode = mapData.nodes.find( currentNode->second.first );
            else
                currentNode = mapData.nodes.find( currentNode->second.second );
            currentInstruction++;
            if( currentInstruction == mapData.instructions.end() )
                currentInstruction = mapData.instructions.begin();
        }
        return numberOfSteps;
    }

    int64_t getMinNumberOfSteps2( const MapData& mapData )
    {
        auto currentNodes = mapData.nodes
            | std::views::filter( [] ( auto& node ) { return node.first[ 2 ] == 'A'; } )
            | std::views::transform( [] ( auto& node ) { return std::pair<NodeId, std::pair<NodeId, NodeId>>{ node.first, { node.second.first, node.second.second } }; } )
            | std::ranges::to<std::vector>();

        auto currentInstruction = mapData.instructions.begin();
        std::vector<int64_t> numberOfSteps( currentNodes.size() );
        for( int64_t nodeId = 0; auto& node : currentNodes )
        {
            while( node.first[ 2 ] != 'Z' )
            {
                numberOfSteps[nodeId]++;
                if( *currentInstruction == 'L' )
                    node = *mapData.nodes.find( node.second.first );
                else
                    node = *mapData.nodes.find( node.second.second );
                currentInstruction++;
                if( currentInstruction == mapData.instructions.end() )
                    currentInstruction = mapData.instructions.begin();
            }
            nodeId++;
        }

        auto result = numberOfSteps.front();
        for( auto& steps : numberOfSteps )
            result = std::lcm( result, steps );
        return result;
    }
}

void executeDay8()
{
    std::ifstream file( "input/Day8.txt" );
    auto mapData = Day8::parseInput( file );
    fmt::print( "Minimum number of steps: {}\n", Day8::getMinNumberOfSteps( mapData ) );
    fmt::print( "Minimum number of steps: {}\n", Day8::getMinNumberOfSteps2( mapData ) );
}
