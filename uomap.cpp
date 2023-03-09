//Copyright © 2023 Charles Kerr. All rights reserved.

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <utility>
#include <array>

#include "uomap.hpp"

using namespace std::string_literals ;

//=========================================================================================
namespace uo {
     const std::array<std::string,uo_map_count> map_names{
        "Felucca"s,"Trammel"s,"Ilshenar"s,"Malas"s,"Tokuno"s,"TerMur"s
    };
    
    const std::array<std::pair<std::uint16_t,std::uint16_t>,uo_map_count> map_size{
        std::make_pair(static_cast<std::uint16_t>(7168),static_cast<std::uint16_t>(4096)),
        std::make_pair(static_cast<std::uint16_t>(7168),static_cast<std::uint16_t>(4096)),
        std::make_pair(static_cast<std::uint16_t>(2304),static_cast<std::uint16_t>(1600)),
        std::make_pair(static_cast<std::uint16_t>(2560),static_cast<std::uint16_t>(2048)),
        std::make_pair(static_cast<std::uint16_t>(1488),static_cast<std::uint16_t>(1488)),
        std::make_pair(static_cast<std::uint16_t>(1280),static_cast<std::uint16_t>(4086))
    };
    
    
    //=========================================================================================
    auto nameForMap(int mapnumber) ->const std::string& {
        if (mapnumber>= uo_map_count){
            throw std::runtime_error("Map number exceeds the maximum number of maps");
        }
        return map_names.at(mapnumber) ;

    }

    
    //=========================================================================================
    auto sizeForMap(int mapnumber) ->const std::pair<std::uint16_t,std::uint16_t>& {
        if (mapnumber>= uo_map_count){
            throw std::runtime_error("Map number exceeds the maximum number of maps");
        }
        return map_size.at(mapnumber) ;
    }
    
    
    //=========================================================================================
    auto calcBlockOffset(int x, int y,std::uint16_t height) -> std::tuple<int,int,int>{
        auto offsety = y%8 ;
        auto offsetx = x%8 ;
        auto block = (y/8)+ ((x/8)*(height/8)) ;
        return std::make_tuple(block,offsetx,offsety) ;
    }
    
    //=========================================================================================
    auto calcCoordForBlock(int blocknum,std::uint16_t height) -> std::pair<int,int> {
        auto xblock = blocknum / (height/8);
        auto yblock = blocknum - (xblock  * (height/8));
        return std::make_pair(xblock*8,yblock*8) ;
    }
    //=========================================================================================
    auto offsetUOPEntry(int x,int y,std::uint16_t height) -> std::pair<int,std::uint64_t> {
        auto [block,offsetx,offsety] = calcBlockOffset(x, y, height) ;
        auto uopentry = block/4096 ;
        auto offset =  static_cast<std::uint64_t>((block - (uopentry*4096)) * uo_map_block_size)+ static_cast<std::uint64_t>(3*8*offsety) + static_cast<std::uint64_t>(offsetx*3);
        return std::make_pair(uopentry,offset) ;
    }

}
