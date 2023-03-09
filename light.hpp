//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef light_hpp
#define light_hpp

#include <cstdint>
#include <string>
#include <vector>

#include "image.hpp"
//=========================================================================================
namespace uo {
    /// Generates an image object from the ptr that points to the raw light data
    /// Parameters:
    /// - ptr: ptr to the raw gump data
    /// - size: size of the data in bytes
    /// - height: the height of the image (normally obtained from the idx entry)
    /// - width: the width of the image (normallh obtained from the idx entry) 
    /// - Returns: an image file of the data
    auto imageForLight(const std::uint8_t *ptr, std::size_t size,std::uint16_t height, std::uint16_t width) ->image_t;

    //=================================================================================
    /// Generates the native uo data format for a texture from an image
     /// Parameters:
    /// - image: the image of the gump
    /// - Returns: a vector of the native uo data for the texture
    auto dataForLight(const image_t &image) ->std::vector<std::uint8_t> ;

}

#endif /* light_hpp */
