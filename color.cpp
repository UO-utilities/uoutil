//Copyright © 2023 Charles Kerr. All rights reserved.

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include "color.hpp"

using namespace std::string_literals ;

//=========================================================================================
namespace uo{
    //=========================================================================================
    auto color_t::convert(std::uint16_t color) ->std::uint32_t {
        constexpr auto factor = 8 ;
        constexpr auto shift = 5 ;
        constexpr auto cshift = 8 ;
        constexpr auto mask = std::uint16_t(0x1f) ;

        auto converted = std::uint32_t(0xFF000000) ;
        
        for (auto j=0 ; j< 3; j++){
            converted |= (((color>>(j*shift)) & mask) * factor)<<(cshift*j);
        }
        if ((color &0x7fff) ==0) {
            converted &= 0xFFFFFF;
        }
        return converted ;
    }
    //=========================================================================================
    auto color_t::convert(std::uint32_t color) ->std::uint16_t {
        constexpr auto factor = 31.0/255.0 ;
        constexpr auto shift = 8 ;
        constexpr auto cshift = 5 ;
        constexpr auto mask = std::uint16_t(0xff) ;

        auto converted = std::uint16_t(0) ;
        
        for (auto j=0 ; j< 3; j++){
            auto temp = (((color>>(j*shift)) & mask) * factor) ;
            converted |= (static_cast<std::uint16_t>((temp<0.9?0:std::round(temp)))<<(cshift*j));
         }
        if (!static_cast<bool>((color>>24) &0xFF) ){
            converted = 0 ;
        }
        return converted ;

    }
    //=========================================================================================
    color_t::color_t():value(0){}
    //=========================================================================================
    color_t::color_t(std::uint16_t color):value(color){}
    //=========================================================================================
    color_t::color_t(std::uint32_t color):value(convert(color)){}

    //=========================================================================================
    auto color_t::transparent() const ->bool {
        return (!static_cast<bool>(value &0x7fff));
    }
    //=========================================================================================
    auto color_t::grey() const ->bool {
        return ((value&0x1f) == ((value>>5)&0x1f)) && ((value&0x1f) == ((value>>10)&0x1f)) ;
    }
    
    //=========================================================================================
    auto color_t::red() const ->int {
        return static_cast<int>((value>>10)&0x1f);
    }
    //=========================================================================================
    auto color_t::green() const ->int {
        return static_cast<int>((value>>5)&0x1f);
    }
    //=========================================================================================
    auto color_t::blue() const ->int {
        return static_cast<int>((value)&0x1f);
    }
    //=========================================================================================
    auto color_t::hue(bool partial,const std::vector<color_t> &colors) const ->color_t {
        auto rvalue = color_t(value) ;
        if (((partial && grey()) || (!partial)) && !transparent()){
            rvalue = colors.at(this->red());
        }
        return rvalue ;
    }
    //=========================================================================================
    auto color_t::hue(bool partial,const std::uint8_t *colors) const ->color_t {
        auto ptr = reinterpret_cast<const std::uint16_t*>(colors);
        auto rvalue = color_t(value) ;
        if (((partial && grey()) || (!partial)) && !transparent()){
            rvalue = *(ptr+this->red());
        }
        return rvalue ;
    }

    //=========================================================================================
    auto color_t::operator=(std::uint16_t color) ->color_t& {
        value = color ;
        return *this;
    }
    //=========================================================================================
    auto color_t::operator=(std::uint32_t color) ->color_t& {
        value = convert(color);
        return *this;
    }
    //=========================================================================================
    auto color_t::operator<(const color_t &color) const ->bool {
        return value < color.value ;
    }

}
