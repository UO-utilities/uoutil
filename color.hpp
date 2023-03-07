//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef color_hpp
#define color_hpp

#include <cstdint>
#include <string>
#include <vector>

//=========================================================================================
namespace uo{
    class color_t {
        std::uint16_t value ;
        static auto convert(std::uint16_t color) ->std::uint32_t ;
        static auto convert(std::uint32_t color) ->std::uint16_t ;

    public:
        color_t();
        color_t(std::uint16_t color);
        color_t(std::uint32_t color);

        auto transparent() const ->bool ;
        auto grey() const ->bool ;
        auto red() const ->int ;
        auto hue(bool partial,const std::vector<color_t> &colors) const ->color_t ;
        
        auto operator=(std::uint16_t color) ->color_t& ;
        auto operator=(std::uint32_t color) ->color_t& ;
    };
    
}
#endif /* color_hpp */
