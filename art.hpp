//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef art_hpp
#define art_hpp

#include <cstdint>
#include <string>

#include "image.hpp"
#include "general.hpp"
#include "uop.hpp"
//=========================================================================================
namespace uo {
    auto imageForTerrain(const std::uint8_t *ptr,std::size_t size) ->image_t ;
    auto dataForTerrain(const image_t &image) ->std::vector<std::uint8_t> ;
    
    auto imageForArt(const std::uint8_t *ptr, std::size_t size) ->image_t ;
    auto dataForArt(const image_t &image) ->std::vector<std::uint8_t> ;
    
    auto hashForTerrain(tileid_t tileid) ->std::uint64_t ;

    auto hashForArt(tileid_t tileid) ->std::uint64_t ;
    
}



#endif /* art_hpp */
