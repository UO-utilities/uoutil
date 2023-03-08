//Copyright © 2023 Charles Kerr. All rights reserved.

#include <iostream>
#include <stdexcept>
#include <map>

#include "art.hpp"
#include "hash.hpp"

using namespace std::string_literals ;

//=========================================================================================
namespace uo {
    
    //=========================================================================================
    // Internal routines
    //=========================================================================================

    //===================================================================================================
    auto lineDataForItem(const image_t &image , int y) ->std::vector<std::uint16_t> {
        auto [height,width] = image.size() ;
        auto line = std::vector<std::uint16_t>();
        auto xoffset = std::uint16_t(0);
        auto run = std::uint16_t(0);
        auto oldcolor = std::uint16_t(0) ;
        auto colors = std::vector<std::uint16_t>() ;
        for (std::uint16_t x = 0 ; x<width;x++){
            auto color = image.color(y,x).value ;
            if (color == 0) {
                if (oldcolor!=0){
                    line.push_back(xoffset);
                    line.push_back(run);
                    for (auto value:colors){
                        line.push_back(value);
                    }
                    oldcolor=0 ;
                    xoffset =0  ;
                }
                xoffset += 1;
                run = 0 ;
                colors.clear();
            }
            else {
                run += 1 ;
                colors.push_back(color) ;
                oldcolor = color ;
            }
        }
        if (!colors.empty()){
            line.push_back(xoffset);
            line.push_back(run) ;
            for (auto value:colors){
                line.push_back(value);
            }
        }
        line.push_back(0);
        line.push_back(0);
        if (line.size()%2==1){
            line.push_back(0);
        }
        return line;
    }

    
    
    //=================================================================================
    auto dataLinesForItem(const image_t &image,std::vector<std::vector<std::uint16_t>> &data) ->std::map<int,int> {
        auto [height,width] = image.size() ;
        auto indicies = std::map<int,int>() ;
        data.clear() ;
        for (auto y = 0;y<height;y++){
            auto line = lineDataForItem(image, y);
            // See if this is all ready in the data
            auto iter = std::find_if(data.begin(),data.end(),[&line](const std::vector<std::uint16_t> &value){
                return line == value ;
            });
            if (iter != data.end()){
                // This line is all ready in our data
                auto index = static_cast<int>(std::distance(data.begin(), iter)) ;
                indicies.insert_or_assign(y, index);
            }
            else {
                // It isn't in our data
                data.push_back(line);
                indicies.insert_or_assign(y, static_cast<int>(data.size()-1));
            }
        }
        return indicies;
    }

    
    //=========================================================================================
    // public routines
    //=========================================================================================

    //=========================================================================================
    auto imageForTerrain(const std::uint8_t *ptr,std::size_t size) ->image_t {
        auto image = image_t(44,44) ;
        auto color = reinterpret_cast<const std::uint16_t *>(ptr) ;
        
        auto run = 2 ;
        auto xloc = 21 ;
        for (auto height = 0 ; height < 22;height++){
            for (auto offset = 0 ; offset < run ; offset++){
                image.color(height,xloc+offset) = *color  ;
                color++ ;
            }
            xloc-- ;
            run +=2 ;
        }
        run = 44 ;
        xloc = 0 ;

        for (auto height = 22 ; height < 44;height++){
            for (auto offset = 0 ; offset < run ; offset++){
                image.color(height,xloc+offset) = *color;
                color++ ;
            }
            xloc++ ;
            run -=2 ;
        }

        
        return image ;
    }
    //=========================================================================================
    auto dataForTerrain(const image_t &image) ->std::vector<std::uint8_t> {
        auto data = std::vector<std::uint8_t>(2024,0) ;
        auto color = reinterpret_cast<std::uint16_t*>(data.data());
        auto run = 2 ;
        auto xloc = 21 ;
        for (auto height = 0 ; height < 22;height++){
            for (auto offset = 0 ; offset < run ; offset++){
                *color = image.color(height,xloc+offset).value  ;
                color++ ;
            }
            xloc-- ;
            run +=2 ;
        }
        run = 44 ;
        xloc = 0 ;
        for (auto height = 22 ; height < 44;height++){
            for (auto offset = 0 ; offset < run ; offset++){
                *color = image.color(height,xloc+offset).value;
                color++ ;
            }
            xloc++ ;
            run -=2 ;
        }
        return data ;
    }
    
    //=========================================================================================
    auto imageForArt(const std::uint8_t *ptr, std::size_t size) ->image_t {
        if (size>8){
            auto unknown = std::uint32_t(0);
            std::copy(ptr,ptr+4,reinterpret_cast<std::uint8_t*>(&unknown));
            auto wdata = reinterpret_cast<const std::uint16_t*>(ptr+4);
            auto width = *wdata ;
            wdata++ ;
            if ((width >0) && (width <1024)) {
                auto height = *wdata ;
                wdata++ ;
                if ((height>0) && (height < 1024)){
                    auto image = image_t(height,width) ;
                    auto x = 0 ;
                    auto y = 0 ;
                    auto offset = reinterpret_cast<const std::uint16_t *>(ptr+8+(height*2) + wdata[y]*2);
                    while (y < height) {
                        auto xoff = *offset ;
                        offset++;
                        auto run = *offset ;
                        offset++;
                        if ((xoff + run) >= 2048) {
                            break;
                        }
                        else if ((xoff+run) != 0){
                            x += xoff ;
                            for (auto j= 0 ; j< run; j++){
                                image.color(y,x+j) = *offset ;
                                offset++ ;
                            }
                            x+= run ;
                        }
                        else {
                            x = 0 ;
                            y++ ;
                            if (y < height){
                                offset = reinterpret_cast<const std::uint16_t *>(ptr+8+(height*2) + wdata[y]*2);
                            }
                        }
                    }
                    return image ;
                }
                else {
                    throw std::runtime_error("Image not avaliable, invalid height.");
                }
            }
            else {
                throw std::runtime_error("Image not avaliable, invalid width.");
            }
        }
        else {
            throw std::runtime_error("Not sufficent data for image.");
        }

    }
    //=========================================================================================
    auto dataForArt(const image_t &image) ->std::vector<std::uint8_t> {
        auto [height,width] = image.size();
        auto data = std::vector<std::vector<std::uint16_t>>() ;
        auto indicies = dataLinesForItem(image, data);
        // Now create an offset vector to match data
        std::vector<std::uint16_t> offsets(data.size(),0) ;
        for (auto j=1 ; j< data.size();j++){
            offsets[j] = static_cast<std::uint16_t>(data[j-1].size()) + offsets[j-1] ;
        }
        auto datasize = 0 ;
        for (const auto &line:data){
            datasize += static_cast<int>(line.size())*2 ;
        }

        // Now we create the vector of data
        auto rvalue = std::vector<uint8_t>(datasize + 4 + 4 + height*2,0); // datasize, 4 byte unknown at beginng, 4 byte width/height, height*2 bytes for offsettable.
        auto startoffset = 4 ;
        std::copy(reinterpret_cast<std::uint8_t*>(&width),reinterpret_cast<std::uint8_t*>(&width)+2,rvalue.begin()+startoffset);
        startoffset += 2 ;
        
        std::copy(reinterpret_cast<std::uint8_t*>(&height),reinterpret_cast<std::uint8_t*>(&height)+2,rvalue.begin()+startoffset);
        startoffset += 2 ;
        for (auto y=0 ; y<height;y++){
            auto value = offsets.at(indicies.at(y)) ;
            std::copy(reinterpret_cast<std::uint8_t*>(&value),reinterpret_cast<std::uint8_t*>(&value)+2,rvalue.begin()+startoffset);
            startoffset += 2 ;
        }
        // And now we copy the data
        for (const auto &line:data){
            std::copy(reinterpret_cast<const std::uint8_t*>(line.data()),reinterpret_cast<const std::uint8_t*>(line.data())+(line.size()*2),rvalue.begin()+startoffset);
            startoffset += static_cast<int>(line.size()*2) ;
        }
        
        return rvalue;

    }
    
    
    //=========================================================================================
    auto hashForTerrain(tileid_t tileid) ->std::uint64_t {
       return uop::hashLittle2(format(uo_hashes.at(uotype_t::art),tileid)) ;
    }
    //=========================================================================================
    auto hashForArt(tileid_t tileid) ->std::uint64_t {
        return uop::hashLittle2(format(uo_hashes.at(uotype_t::art),static_cast<std::uint32_t>(tileid)+0x4000)) ;
    }

}
