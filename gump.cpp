//Copyright © 2023 Charles Kerr. All rights reserved.

#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "gump.hpp"
#include "general.hpp"
#include "hash.hpp"
using namespace std::string_literals ;

//=========================================================================================
namespace uo {
    //=================================================================================
    // internal routines used during the conversion
    //=================================================================================

    //=================================================================================
    /// Generates a RLE type format of data for a single row in a gump image
    /// Parameters:
    /// - image: the image of the gump
    /// - y: the row that one is requesting be processed
    /// - Returns: a vector of RLE data for the row
    auto dataForGumpLine(const image_t &image, int y) ->std::vector<std::uint16_t> {
        auto temp = std::vector<std::uint16_t>() ;
        auto [height,width] = image.size() ;
        auto run = 0 ;
        auto color = std::uint16_t(0) ;
        for (auto j=0;j<width;j++){
            if (j == 0 ){
                color = image.color(y,j).value;
                run = 1 ;
            }
            else {
                if (color == (image.color(y,j).value )){
                    run++ ;
                }
                else {
                    temp.push_back(color) ;
                    temp.push_back(run) ;
                    run = 1 ;
                    color =  (image.color(y,j).value) ;
                }
            }
        }
        temp.push_back(color);
        temp.push_back(run) ;

        return temp ;
    }
    //=================================================================================
    // Public routines
    //=================================================================================
    //=================================================================================
    auto imageForGump(const std::uint8_t *ptr, std::size_t size) ->image_t {
        if (size <= 8){
            return image_t();  // This is invalid (some uops have this), so lets just return an empty image
        }
        auto width = std::uint32_t(0);
        auto height = std::uint32_t(0) ;
        std::copy(ptr,ptr+4,reinterpret_cast<std::uint8_t*>(&width));
        std::copy(ptr+4,ptr+8,reinterpret_cast<std::uint8_t*>(&height));
        return imageForGump(ptr+8,size-8,height,width) ;
    }
    
    //=================================================================================
    auto imageForGump(const std::uint8_t *ptr, std::size_t size,std::uint32_t height, std::uint32_t width) ->image_t {
        auto image = image_t(height,width) ;
        auto offsets = std::vector<std::uint32_t>(height,0) ;
        auto ptroffset = reinterpret_cast<const std::uint32_t*>(ptr);
        for (std::uint32_t j=0 ; j< height; j++){
            offsets[j] = *(ptroffset+j) ;
        }
        auto blocksize = 0 ;
        auto run = std::uint16_t(0) ;
        auto color = std::uint16_t(0) ;
        
        for (std::uint32_t y = 0 ; y<height;y++){
            if (y < height-1){
                blocksize = offsets[y+1] - offsets[y] ;
            }
            else {
                blocksize = static_cast<std::uint32_t>(size/4) - offsets[y];
            }
            auto pos =  (offsets[y] * 4)  ;
            
            auto x = 0 ;
            for (auto entry = 0 ; entry < blocksize ; entry++){
                std::copy(ptr+pos,ptr+pos+2,reinterpret_cast<char*>(&color));
                std::copy(ptr+pos+2,ptr+pos+4,reinterpret_cast<char*>(&run));
                pos += 4 ;
                for (std::uint16_t j = 0 ; j < run ; j++){
                    image.color( y,x) = color;
                    x++ ;
                }
            }
        }
        return image ;
    }

    //=================================================================================
    auto dataForGump(const image_t &image,bool prependsize) ->std::vector<std::uint8_t> {
        auto [height,width] = image.size() ;
        auto lines = std::vector<std::vector<std::uint16_t>>(height);
        // Gather up all the lines of the gump
        for (auto y = 0 ;y < height;y++){
            lines[y] = dataForGumpLine(image, y);
        }
        // Now lets create the offset table
        auto offsets = std::vector<std::uint32_t>(height,0) ;
        auto base = static_cast<std::uint32_t>(height)  ;
        for (auto j=0 ; j<height;j++){
            offsets[j] = base ;
            base += static_cast<std::uint32_t>(lines[j].size())/2 ;
        }
        //
        auto sizes = std::vector<std::uint32_t>(2,0) ;
        sizes[0]= static_cast<std::uint32_t>(width) ;
        sizes[1] = static_cast<std::uint32_t>(height);
        
        auto total = offsets.size()*4  ;
        if (prependsize){
            total+= (sizes.size()*4);
        }
        for (const auto &line:lines){
            total += line.size()*2 ;
        }
        
        auto data = std::vector<std::uint8_t>(total,0) ;
        auto cpoffset = 0;
        if (prependsize){
            // copy the sizes
            std::copy(reinterpret_cast<std::uint8_t*>(sizes.data()),reinterpret_cast<std::uint8_t*>(sizes.data())+8,data.begin() );
           cpoffset = 8 ;
        }
        // Copy the offsets
        std::copy(reinterpret_cast<std::uint8_t*>(offsets.data()),reinterpret_cast<std::uint8_t*>(offsets.data())+offsets.size()*4,data.begin()+cpoffset );
        cpoffset += static_cast<std::int32_t>(offsets.size())*4 ;
        for (const auto &line:lines){
            std::copy(reinterpret_cast<const std::uint8_t*>(line.data()),reinterpret_cast<const std::uint8_t*>(line.data())+line.size()*2,data.begin()+cpoffset );
            cpoffset += static_cast<std::int32_t>(line.size())*2;
        }
        return data ;
    }

    
    //=================================================================================
    auto hashForGump(std::uint32_t gumpid) ->std::uint64_t{
        return uop::hashLittle2(format(uo_hashes.at(uotype_t::gump),gumpid)) ;
    }
}
