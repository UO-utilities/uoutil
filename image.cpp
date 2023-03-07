//Copyright © 2023 Charles Kerr. All rights reserved.

#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "image.hpp"

using namespace std::string_literals ;

//=========================================================================================
namespace uo {
    
    //=========================================================================================
    image_t::image_t(std::size_t height, std::size_t width,const color_t &fill) :image_t(){
        data = std::vector<std::vector<color_t>>(height,std::vector<color_t>(width,fill));
    }
    //=========================================================================================
    image_t::image_t(std::uint32_t height, std::uint32_t width,const color_t &fill):image_t(static_cast<std::size_t>(height),static_cast<std::size_t>(width),fill){}
    
    //=========================================================================================
    auto image_t::empty() const ->bool {
        auto rvalue = data.empty();
        if (!rvalue) {
            rvalue = data.at(0).empty();
        }
        return rvalue;
    }

    //=========================================================================================
    auto image_t::size() const ->std::pair<std::size_t,std::size_t>{
        auto width = std::size_t(0);
        auto height =  data.size();
        if (!data.empty()){
            width = data.at(0).size();
        }
        return std::make_pair(height,width);
    }
    
    //=========================================================================================
    auto image_t::size(std::size_t height, std::size_t width,const color_t &fill) -> image_t& {
        for (auto &entry:data){
            entry.resize(width,fill);
        }
        data.resize(height,std::vector<color_t>(width,fill));
        return *this;
    }

    //=========================================================================================
    auto image_t::color(std::size_t row, std::size_t col) const -> const color_t& {
        return data.at(row).at(col);
    }
    //=========================================================================================
    auto image_t::color(std::size_t row, std::size_t col)  ->  color_t& {

        return data.at(row).at(col);
    }
    
    //=========================================================================================
    auto image_t::flip() ->image_t& {
        for (auto &entry:data){
            std::reverse(entry.begin(), entry.end());
        }
        return *this;
    }
    //=========================================================================================
    auto image_t::invert() ->image_t &{
        std::reverse(data.begin(), data.end());
        return *this;
    }
    //=========================================================================================
    auto image_t::hue(bool partial , const std::vector<color_t> &colors) const -> image_t {
        auto [height,width] = this->size();
        auto rvalue = image_t(height,width);
        for (auto row = 0 ; row < height;row++){
            for (auto col = 0 ; col < width;col++){
                rvalue.color(row,col) = this->color(row,col).hue(partial,colors);
            }
        }
        return rvalue ;
    }
}
