//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef idx_hpp
#define idx_hpp

#include <cstdint>
#include <string>
#include <limits>
//=========================================================================================
namespace uo {
    
    /// Entry in an UO IDX file
    struct idx_entry{
        std::uint32_t offset ;
        std::uint32_t length ;
        std::uint32_t extra ;
        /// Constructor for an idx entry
        /// Parameters:
        ///     - file_offset: offset into the data file
        ///     - data_length: length of the data
        ///     - extra_data: meaning varies. Textures it is 1 if 128x128, in art it is 0xFFFFF if no artwork, in gumps is width/height
        idx_entry(std::uint32_t file_offset = 0,std::uint32_t data_length = std::numeric_limits<std::uint32_t>::max(),std::uint32_t extra_data = 0);
        /// Constructor for an idx entry
        /// Parameters:
        ///     - input: input stream for an idx file
        idx_entry(std::istream &input);
        
        /// indicates if the data pointed to by this entry is valid (length > 0) ;
        /// - Returns: true if valid
        auto valid() const ->bool ;
        
        /// For textures, indicates if the data represents a 64x64 bit texture
        /// - Returns: true 64x64
        auto tex64() const ->bool ;
        /// For textures, indicates if the data represents a 128x128 bit texture
        /// - Returns: true 128x128
        auto tex128() const ->bool ;
        /// For Lights, returns the width of the image
        /// - Returns: width of the image
        auto lightWidth() const ->std::uint16_t ;
        /// For Lights, returns the hieght of the image
        /// - Returns: height of the image
        auto lightHeight() const ->std::uint16_t ;
        /// writes the entry to the stream
        /// Parameters:
        ///     - output: the stream to the idx file
        auto save(std::ostream &output) const ->void ;
        /// Reads the entry from the stream
        /// Parameters:
        ///     - input: the stream to the idx file
        auto read(std::istream &input) ->void ;
    };
}
#endif /* idx_hpp */
