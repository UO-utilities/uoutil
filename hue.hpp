//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef hue_hpp
#define hue_hpp

#include <cstdint>
#include <string>
#include <vector>
#include <istream>

#include "general.hpp"
/*
 3.7 HUES.MUL
 Just read in HueGroups until you hit the end of the file. Note that large chunks of this file consist of garbage--OSI admits to this (something about a bug in their old code).
 
 If you want to look at the hues, check out this.
 
 Hues are applied to an image in one of two ways. Either all gray pixels are mapped to the specified color range (resulting in a part of an image changed to that color) or all pixels are first mapped to grayscale and then the specified color range is mapped over the entire image.
 
 HueEntry
 WORD ColorTable[32];
 WORD TableStart;
 WORD TableEnd;
 CHAR Name[20];
 
 HueGroup
 DWORD Header;
 HueEntry Entries[8];
 */
//=========================================================================================
namespace uo {
    
    class color_t ;
    /// Maximum number of hues the client supports
    constexpr auto max_hue_entry = 3000 ;
    
    /// Calculate the offset for a given hueid
    /// Parameters:
    /// - hueid: the id of the desired hue
    /// - Returns: the offset to the start of the hue table for the hue id
    auto offsetForHue(hueid_t hueid) ->std::uint64_t ;
    
    /// Return a vector of color objects for a given hue id
    /// Parameters:
    /// - input: stream for the hue data file
    /// - hueid: the id of the hue one wants
    /// - Returns: vector of color objects (32 entries)
    auto colorsForHue(std::istream &input,hueid_t hueid) ->std::vector<color_t> ;
    
    /// Return a vector of color objects for a given hue id
    /// This routine would not normally be used, as one can just directly reference the color
    /// via the pointer, which would be faster
    /// Parameters:
    /// - ptr: ptr to the beginning of the hue data file (NOT entry)
    /// - size: The size of the data
    /// - hueid: the id of the hue one wants
    /// - Returns: vector of color objects (32 entries)
    auto colorsForHue(const std::uint8_t *ptr,std::size_t size, hueid_t hueid) ->std::vector<color_t> ;

    /// Write the colors to a hue entry.  Will set the start and stop entries
    /// Parameters:
    /// - output: stream to the hue data file
    /// - hueid: the id of the hue one wants
    /// - colors: a vector of colors to write
    /// - Returns: nothing
    auto setColorsForHue(std::ostream &output,hueid_t hueid,const std::vector<color_t> &colors) ->void ;
    
    /// Write the colors to a hue entry.  Will set the start and stop entries
    /// Parameters:
    /// - ptr: pointer to the hue data file
    /// - size: size of the hue data
    /// - hueid: the id of the hue one wants
    /// - colors: a vector of colors to write
    /// - Returns: nothing
    auto setColorsForHue(std::uint8_t *ptr , std::size_t size, hueid_t hueid,const std::vector<color_t> &colors) ->void ;

    /// Obtain the text associated with a hue
    /// Parameters:
    /// - input: stream to the hue data file
    /// - hueid: the id of the hue one wants
    /// - Returns: string of the text
    auto stringForHue(std::istream &input,hueid_t hueid) ->std::string ;
    
    /// Obtain the text associated with a hue
    /// Parameters:
    /// - ptr: pointer to the hue data file
    /// - size: size to the hue data
    /// - hueid: the id of the hue one wants
    /// - Returns: string of the text
    auto stringForHue(const std::uint8_t *ptr,std::size_t size,hueid_t hueid) ->std::string ;

    /// set the text associated with a hue
    /// Parameters:
    /// - output: stream to the hue data file
    /// - hueid: the id of the hue one wants
    /// - text: string to set for the hueid
    /// - Returns: nothing
    auto setStringForHue(std::ostream &output,hueid_t hueid,const std::string &text) ->void ;

    /// set the text associated with a hue
    /// Parameters:
    /// - ptr: pointer to the hue data
    /// - size: size of the hue data
    /// - hueid: the id of the hue one wants
    /// - text: string to set for the hueid
    /// - Returns: nothing
    auto setStringForHue(std::uint8_t *ptr,std::size_t size, hueid_t hueid,const std::string &text) ->void;
}
#endif /* hue_hpp */
