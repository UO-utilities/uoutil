//Copyright © 2023 Charles Kerr. All rights reserved.

#include <iostream>
#include <stdexcept>

#include "light.hpp"

using namespace std::string_literals ;

//=========================================================================================
namespace uo {
    //=================================================================================
    auto imageForLight(const std::uint8_t *ptr, std::size_t size,std::uint16_t height, std::uint16_t width) ->image_t {
        auto image =image_t(height,width) ;
        
        for (std::uint16_t y=0 ; y<height;y++){
            for (std::uint16_t x=0 ; x<width ; x++){
                auto chan = static_cast<std::uint16_t>(*ptr) ;
                auto channel = chan & 0x1f;
                image.color(y,x).value = ((channel<<10) | (channel<<5) | (channel));
                ptr++;
            }
        }
        return image ;
    }
    //=================================================================================
    auto dataForLight(const image_t &image) ->std::vector<std::uint8_t> {
        auto [height,width]  = image.size();
        auto data = std::vector<std::uint8_t>(width*height,0);
        auto value = data.begin();
        for (std::size_t y=0 ; y<height;y++){
            for (std::size_t x=0 ; x< width;x++){
                auto channel = static_cast<std::uint8_t>((image.color(y,x).value & 0x1f));
                *value = channel ;
                value++;
            }
        }
        return data ;
        
    }


}
