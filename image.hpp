//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef image_hpp
#define image_hpp

#include <cstdint>
#include <string>
#include <vector>
#include <utility>
#include <filesystem>

#include "color.hpp"
//=========================================================================================
namespace uo {
    class image_t {
        std::vector<std::vector<color_t>> data ;
        
    public:
        image_t() = default ;
        image_t(std::size_t height, std::size_t width,const color_t &fill = color_t()) ;
        image_t(std::uint32_t height, std::uint32_t width,const color_t &fill = color_t());
        auto empty() const ->bool ;
        
        auto size() const ->std::pair<std::size_t,std::size_t>;
        auto size(std::size_t height, std::size_t width,const color_t &fill = color_t()) -> image_t& ;
        
        auto color(std::size_t row, std::size_t col) const -> const color_t& ;
        auto color(std::size_t row, std::size_t col)  ->  color_t& ;
        
        auto flip() ->image_t& ;
        auto invert() ->image_t &;
        auto hue(bool partial , const std::vector<color_t> &colors) const -> image_t ;
        auto hue(bool partial , const std::uint8_t *colors) const -> image_t ;
    };
    
    auto loadBMP(const std::filesystem::path &path) ->image_t ;
    auto saveBMP(const image_t &image, const std::filesystem::path &path) ->bool ;
}
#endif /* image_hpp */
