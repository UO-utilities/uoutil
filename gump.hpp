//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef gump_hpp
#define gump_hpp

#include <cstdint>
#include <string>
#include <vector>

#include "image.hpp"
//=========================================================================================
namespace uo {
    
    /// Generates an image object from the ptr that points to the raw gump data
    /// This method assumes the width/height is part of the data stream (uop format)
    /// Parameters:
    /// - ptr: ptr to the raw gump data
    /// - size: size of the data in bytes
    /// - Returns: an image file of the data
    auto imageForGump(const std::uint8_t *ptr, std::size_t size) ->image_t;
    /// Generates an image object from the ptr that points to the raw gump data
    /// The width/height is provided externally (in the idx entry), and not part of the data stream (mul format)
    /// Parameters:
    /// - ptr: ptr to the raw gump data
    /// - size: size of the data in bytes
    /// - height: height of the image
    /// - width: width of the image
    /// - Returns: an image file of the data
    auto imageForGump(const std::uint8_t *ptr, std::size_t size,std::uint32_t height, std::uint32_t width) ->image_t;

    //=================================================================================
    /// Generates the native uo data format for a gump from an image
    /// Optionally one can have the width and height prepended to the data (uop format)
    /// Parameters:
    /// - image: the image of the gump
    /// - prependsize: if true will prepend the width and height informat, usable for uop format
    /// - Returns: a vector of the native uo data for the gump
    auto dataForGump(const image_t &image,bool prependsize) ->std::vector<std::uint8_t> ;

    //=================================================================================
    /// Generates a hash value for a gump id
    /// Parameters:
    /// - gumpid: the id of the gump
    /// - Returns: a hash value for the gump
    auto hashForGump(std::uint32_t gumpid) ->std::uint64_t ;

}
#endif /* gump_hpp */
