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
    /// converts a data pointed to in a mul/uop to a image
    /// Parameters:
    /// - ptr: ptr to the data of the art data
    /// - size: the amount of data
    /// - Returns: An image object of the data
    auto imageForTerrain(const std::uint8_t *ptr,std::size_t size) ->image_t ;
    /// converts to data an image of terrain art
    /// Parameters:
    /// - image: the artwork for the image file
    /// - Returns: A vector of converted data (suitable for storing in a mul/uop)
    auto dataForTerrain(const image_t &image) ->std::vector<std::uint8_t> ;
    
    /// converts a data pointed to in a mul/uop to a image
    /// Parameters:
    /// - ptr: ptr to the data of the art data
    /// - size: the amount of data
    /// - Returns: An image object of the data
    auto imageForArt(const std::uint8_t *ptr, std::size_t size) ->image_t ;
    /// converts to data an image of art tile
    /// Parameters:
    /// - image: the artwork for the image file
    /// - Returns: A vector of converted data (suitable for storing in a mul/uop)
    auto dataForArt(const image_t &image) ->std::vector<std::uint8_t> ;
    
    /// Generates the hash needed for a terrain tile artwork
    /// Parameters:
    /// - tileid: The terrain tile id
    /// - Returns: the hash value for that tile (useable in an uop)
    auto hashForTerrain(tileid_t tileid) ->std::uint64_t ;

    /// Generates the hash needed for a art tile artwork
    /// Parameters:
    /// - tileid: The art tile id  (not the system tileid , so Not offset by 0x4000). This ranges from 0-0xFFFE
    /// - Returns: the hash value for that tile (useable in an uop)
    auto hashForArt(tileid_t tileid) ->std::uint64_t ;
    
}



#endif /* art_hpp */
