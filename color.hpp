//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef color_hpp
#define color_hpp

#include <cstdint>
#include <string>
#include <vector>

//=========================================================================================
namespace uo{
    class color_t {
        /// Converts a 16 bit color valule to a 32 bit value.
        /// 16 alpha bit is not honored. If the 16 bit value is transparent(all 0s)
        /// the alpha for the 32 bit value is transparent
        /// Otherwise it is set to opaque
        /// - Returns:
        ///     - a 32 bit value for color 
        static auto convert(std::uint16_t color) ->std::uint32_t ;
        /// Converts a 32 bit color valule to a 16 bit value.
        /// If the alpha for the 32 bit color is less then 128,
        /// the 16 bit value is set to all 0s (transparent)
        /// - Returns:
        ///     - a 16 bit value for color
        static auto convert(std::uint32_t color) ->std::uint16_t ;

    public:
        std::uint16_t value ;
        
        /// Constructor for a color object
        color_t();
        /// Constructor for a color object
        /// Parameters:
        ///     - color: Allows onw to specify a 16 bit color for the value
        color_t(std::uint16_t color);
        /// Constructor for a color object
        /// Parameters:
        ///     - color: Allows onw to specify a 32 bit color for the value
        color_t(std::uint32_t color);

        /// Indicates if the color is transparent (all channels 0)
        /// - Returns: true if all 0's
        auto transparent() const ->bool ;
        /// Indicates if the color is a gray value (all channels equal)
        /// - Returns: true if all red==blue==green
        auto grey() const ->bool ;
        /// Provides the red channel value (0-31)
        /// - Returns: red channel value (0-31)
        auto red() const ->int ;
        /// Provides the green channel value (0-31)
        /// - Returns: green channel value (0-31)
        auto green() const ->int ;
        /// Provides the blue channel value (0-31)
        /// - Returns: blue channel value (0-31)
        auto blue() const ->int ;
        /// Provides a hue colored value from the original
        /// Takes the red channel value as a lookup into the vector colors
        /// If partial, only if the color is grey, will it perfrom the lookup
        /// Parameters:
        ///     - partial: Indicator if a partial (only grey values) colors should be used
        ///     - colors: A vector of colors to chose from (0-31) entries.
        /// - Returns: The color from the vector of colors provided
        auto hue(bool partial,const std::vector<color_t> &colors) const ->color_t ;
        /// Provides a hue colored value from the original
        /// Takes the red channel value as a lookup into the ptr pointing to 16 bit colors
        /// If partial, only if the color is grey, will it perfrom the lookup
        /// Parameters:
        ///     - partial: Indicator if a partial (only grey values) colors should be used
        ///     - colors: a pointer to an array of 16 bit colors ;
        /// - Returns: The color from the vector of colors provided
        auto hue(bool partial,const std::uint8_t *colors) const ->color_t ;

        /// Allows one to set the objet with a 16 bit color value ;
        /// Parameters:
        ///     - color: A 16 bit color value
        ///     - Returns: a reference to itself
        auto operator=(std::uint16_t color) ->color_t& ;
        /// Allows one to set the objet with a 32 bit color value ;
        /// Parameters:
        ///     - color: A 32 bit color value
        ///     - Returns: a reference to itself
        auto operator=(std::uint32_t color) ->color_t& ;
        /// Allows one to compare two colors, useful to allow use in std::set ;
        /// Parameters:
        ///     - color: a color object to compare to
        ///     - Returns: true if less then
        auto operator<(const color_t &color) const ->bool ;
    };
    
}
#endif /* color_hpp */
