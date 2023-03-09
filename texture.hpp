//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef texture_hpp
#define texture_hpp

#include <cstdint>
#include <string>
#include <vector>

#include "image.hpp"
//=========================================================================================
namespace uo {
    
    /// Generates an image object from the ptr that points to the raw texture data
    /// Parameters:
    /// - ptr: ptr to the raw gump data
    /// - size: size of the data in bytes
    /// - Returns: an image file of the data
    auto imageForTexture(const std::uint8_t *ptr, std::size_t size) ->image_t;

    //=================================================================================
    /// Generates the native uo data format for a texture from an image
     /// Parameters:
    /// - image: the image of the gump
    /// - Returns: a vector of the native uo data for the texture
    auto dataForTexture(const image_t &image) ->std::vector<std::uint8_t> ;


}
#endif /* texture_hpp */
