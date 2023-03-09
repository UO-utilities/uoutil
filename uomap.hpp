//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef uomap_hpp
#define uomap_hpp

#include <cstdint>
#include <string>
#include <tuple>
//=========================================================================================
namespace uo {
    
    
    constexpr auto uo_map_count = 6;
    
    constexpr auto uo_map_block_size = (8*8*3)+4 ;

    /// Provide the name for a given map
    /// Parameters:
    /// - mapnumber: The map number (0-5) ;
    /// - Returns: reference to a const string that contains the name
    auto nameForMap(int mapnumber) ->const std::string& ;
    
    /// Provide the map size (width,height) for a given map
    /// Parameters:
    /// - mapnumber: The map number (0-5) ;
    /// - Returns: a pair that of unsigned shorts that are the width and height
    auto sizeForMap(int mapnumber) ->const std::pair<std::uint16_t,std::uint16_t>& ;
    
    /// Calculates the block number and the x,y offset into that block for a given map x,y
    /// Parameters:
    /// - x: location in the map in the x axis
    /// - y: location in the map in the y axis
    /// - height: The height of map (NOT z, but max y)
    /// - Returns: the block number, the x offset, and the y offset
    auto calcBlockOffset(int x, int y,std::uint16_t height) -> std::tuple<int,int,int> ;
    
    /// Calculates the map x,y coordinate for the start of a block
    /// Parameters:
    /// - blocknum: the block number
    /// - height: The height of map (NOT z, but max y)
    /// - Returns: the map x and y coordinate for that block
    auto calcCoordForBlock(int blocknum,std::uint16_t height) -> std::pair<int,int> ;
    
    /// Calculates the map x,y coordinate for the start of a block
    /// Parameters:
    /// - x: the x postion in the map
    /// - y: The y position in the map
    /// - height: The height of the map
    /// - Returns: the uop entry and offset from the start of that entry ;
    auto offsetUOPEntry(int x,int y,std::uint16_t height) -> std::pair<int,std::uint64_t> ;

}
#endif /* uomap_hpp */
