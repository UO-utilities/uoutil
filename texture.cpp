//Copyright © 2023 Charles Kerr. All rights reserved.

#include <iostream>
#include <stdexcept>

#include "texture.hpp"

using namespace std::string_literals ;

//=========================================================================================
namespace uo{
    //=================================================================================
    auto imageForTexture(const std::uint8_t *ptr, std::size_t size) ->image_t  {
        auto width = 64 ;
        if (size == 32768){
            width = 128;
        }
        auto color = reinterpret_cast<const std::uint16_t *>(ptr) ;
        auto image =image_t(width,width) ;
        for (auto y=0 ; y < width ; y++){
            for (auto x=0 ; x< width ; x++){
                image.color(y,x) = *color;
                color++ ;
            }
        }
        return image ;
    }
    
    //=================================================================================
    auto dataForTexture(const image_t &image) ->std::vector<std::uint8_t> {
        auto [height,width] = image.size() ;
        auto data = std::vector<std::uint8_t>(width*height*2,0);
        auto color = reinterpret_cast<std::uint16_t *>(data.data()) ;
        
        for (auto y=0; y<height;y++){
            for (auto x=0; x<width;x++){
                *color = image.color(y,x).value;
                color++ ;
            }
        }
        return data ;
    }
}
