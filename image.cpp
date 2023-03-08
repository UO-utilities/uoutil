//Copyright © 2023 Charles Kerr. All rights reserved.

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <fstream>
#include <array>

#include "image.hpp"

using namespace std::string_literals ;

//=========================================================================================
namespace uo {
    
    //=========================================================================================
    image_t::image_t(std::size_t height, std::size_t width,const color_t &fill) :image_t(){
        data = std::vector<std::vector<color_t>>(height,std::vector<color_t>(width,fill));
    }
    
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
    //=========================================================================================
    auto image_t::hue(bool partial , const std::uint8_t *colors) const -> image_t {
        auto [height,width] = this->size();
        auto rvalue = image_t(height,width);
        for (auto row = 0 ; row < height;row++){
            for (auto col = 0 ; col < width;col++){
                rvalue.color(row,col) = this->color(row,col).hue(partial,colors);
            }
        }
        return rvalue ;
    }
    
    
    //============================================================================
    struct bmpsig{
        constexpr static auto size = 14 ;
        std::uint16_t signature ;
        std::uint32_t filesize ;
        std::uint32_t placeholder;
        std::uint32_t offset ;
        [[maybe_unused]] auto load(std::ifstream &input) ->bmpsig& {
            input.read(reinterpret_cast<char*>(&signature),sizeof(signature));
            input.read(reinterpret_cast<char*>(&filesize),sizeof(filesize));
            input.read(reinterpret_cast<char*>(&placeholder),sizeof(placeholder));
            input.read(reinterpret_cast<char*>(&offset),sizeof(offset));
            return *this;
        }
        bmpsig():filesize(0),placeholder(0),offset(14+40),signature(0x4d42){
        }
        bmpsig(std::ifstream &input){
            load(input) ;
        }
        auto save(std::ofstream &output) ->void {
            output.write(reinterpret_cast<char*>(&signature),sizeof(signature));
            output.write(reinterpret_cast<char*>(&filesize),sizeof(filesize));
            output.write(reinterpret_cast<char*>(&placeholder),sizeof(placeholder));
            output.write(reinterpret_cast<char*>(&offset),sizeof(offset));
        }
    };
    
    //============================================================================
    struct dibheader{
        constexpr static auto size = 40 ;
        std::uint32_t header_size ;
        std::int32_t width ;
        std::int32_t height ;
        std::uint16_t num_planes ;
        std::uint16_t bits_pixel ;
        std::uint32_t compression ;
        std::uint32_t image_size ;
        std::uint32_t horiz_resolution ;
        std::uint32_t vert_resolution ;
        std::uint32_t color_count ;
        std::uint32_t important_count ;
        [[maybe_unused]] auto load(std::ifstream &input) ->dibheader& {
            input.read(reinterpret_cast<char*>(&header_size),sizeof(header_size));
            if (header_size < dibheader::size){
                throw std::runtime_error("Unsupported dib header format");
            }
            input.read(reinterpret_cast<char*>(&width),sizeof(width));
            input.read(reinterpret_cast<char*>(&height),sizeof(height));
            input.read(reinterpret_cast<char*>(&num_planes),sizeof(num_planes));
            input.read(reinterpret_cast<char*>(&bits_pixel),sizeof(bits_pixel));
            input.read(reinterpret_cast<char*>(&compression),sizeof(compression));
            input.read(reinterpret_cast<char*>(&image_size),sizeof(image_size));
            input.read(reinterpret_cast<char*>(&horiz_resolution),sizeof(horiz_resolution));
            input.read(reinterpret_cast<char*>(&vert_resolution),sizeof(vert_resolution));
            input.read(reinterpret_cast<char*>(&color_count),sizeof(color_count));
            input.read(reinterpret_cast<char*>(&important_count),sizeof(important_count));
            return *this;
        }
        auto save(std::ofstream &output) ->void {
            output.write(reinterpret_cast<char*>(&header_size),sizeof(header_size));
            output.write(reinterpret_cast<char*>(&width),sizeof(width));
            output.write(reinterpret_cast<char*>(&height),sizeof(height));
            output.write(reinterpret_cast<char*>(&num_planes),sizeof(num_planes));
            output.write(reinterpret_cast<char*>(&bits_pixel),sizeof(bits_pixel));
            output.write(reinterpret_cast<char*>(&compression),sizeof(compression));
            output.write(reinterpret_cast<char*>(&image_size),sizeof(image_size));
            output.write(reinterpret_cast<char*>(&horiz_resolution),sizeof(horiz_resolution));
            output.write(reinterpret_cast<char*>(&vert_resolution),sizeof(vert_resolution));
            output.write(reinterpret_cast<char*>(&color_count),sizeof(color_count));
            output.write(reinterpret_cast<char*>(&important_count),sizeof(important_count));
           
        }
        //Print resolution of the image,
        //72 DPI × 39.3701 inches per metre yields 2834.6472
        dibheader():header_size(size),width(0),height(0),num_planes(1),bits_pixel(32),compression(0),image_size(0),horiz_resolution(0xb13),vert_resolution(0xb13),color_count(0),important_count(0) {
        }
        dibheader(std::ifstream &input){
            load(input) ;
        }
    };
    
    
    //=========================================================================================
    auto loadBMP(const std::filesystem::path &path) ->image_t {
        auto palette= std::vector<color_t>() ;
        auto input = std::ifstream(path.string(),std::ios::binary);
        if (!input.is_open()){
            throw std::runtime_error("Unable to open: "s + path.string());
        }
        auto header = bmpsig(input);
        if (header.signature != 0x4d42){
            throw std::runtime_error("Invalid bmp signature: "s + path.string());
        }
        auto dib = dibheader(input);
        if (dib.header_size < dibheader::size){
            throw std::runtime_error("Invalid dib header size in bmp: "s+path.string());
        }
        if (((dib.bits_pixel != 8) && (dib.bits_pixel != 16) && (dib.bits_pixel != 24) && (dib.bits_pixel != 32)) || (dib.compression!=0)){
            throw std::runtime_error("Unsupported bmp, must be 8,16,24,32 bit with no compression: "s+path.string());
        }
        auto pad = 0 ;
        switch (dib.bits_pixel){
            case 8:{
                pad = (((dib.width )%4)>0? 4-((dib.width )%4):0) ;
                break;
            }
            case 16:{
                pad = (((dib.width * 2)%4)>0? 4-((dib.width * 2)%4):0) ;
                break;
            }
            case 24:{
                pad = (((dib.width * 3)%4)>0? 4-((dib.width * 3)%4):0) ;
                break;
            }
            case 32:{
                pad = 0;
                break;
            }
            default: {
                throw std::runtime_error("Unsupported bmp, must be 8,16,24,32 bit with no compression: "s+path.string());
                break;
            }
        }
        // Do we have anymore header to skip?
        input.seekg((dib.header_size - dibheader::size),std::ios::cur);
        auto current = static_cast<std::uint32_t>(input.tellg());
        if (current != header.offset){
            // We need to read the palette
            auto palette_data = std::array<std::uint8_t,4>() ;
            for (std::uint32_t j= 0 ; j<(header.offset-current)/4;j++){
                input.read(reinterpret_cast<char*>(palette_data.data()),palette_data.size());
                auto color = std::uint32_t(0) ;
                color |=  ( (palette_data[3]<<24)|(palette_data[2]<<16) | (palette_data[1] << 8) | palette_data[0]) ;
                palette.push_back(color_t(color));
            }
        }
        input.seekg(header.offset,std::ios::beg);
        auto width = std::abs(dib.width) ;
        auto height = std::abs(dib.height);
        auto image = image_t(static_cast<std::size_t>(height),static_cast<std::size_t>(width));
        
        for (auto row = 0 ; row< std::abs(dib.height);row++){
            for (auto col = 0 ; col < std::abs(dib.width);col++){
                auto color = color_t() ;
                switch(dib.bits_pixel){
                    case 8:{
                        auto value = std::uint8_t(0);
                        input.read(reinterpret_cast<char*>(&value),1);
                        color = palette.at(value);
                        break;
                    }
                    case 16:{
                        auto value = std::uint16_t(0);
                        input.read(reinterpret_cast<char*>(&value),2);
                        color = value ;
                        break;
                    }
                    case 24:{
                        auto value = std::uint32_t(0xFF000000);
                        auto data = std::array<std::uint8_t,3>();
                        input.read(reinterpret_cast<char*>(data.data()),3);
                        value |= ((data.at(0)<<16) | (data.at(1)<<8) | data.at(3));
                        color = value ;
                        break;
                    }
                    case 32:{
                        auto value = std::uint32_t(0);
                        input.read(reinterpret_cast<char*>(&value),4);
                        color = value ;
                        break;
                    }
                        
                }
                image.color(height-row-1,col) = color ;
            }
            input.seekg(pad,std::ios::cur);
        }
        
        
        return image ;
    }
    //=========================================================================================
    auto saveBMP(const image_t &image, const std::filesystem::path &path) ->bool {
        auto output = std::ofstream(path.string(),std::ios::binary);
        auto rvalue = output.is_open();
        if (rvalue){
            auto [height,width] = image.size();
            auto pad  = (((width * 2)%4)>0? 4-((width * 2)%4):0) ;
            auto paddata = std::vector<char>(pad,0);
            auto sig = bmpsig();
            auto dib = dibheader();
            dib.bits_pixel=16;
            auto imagesize = (width*2 + width*pad) * height ;
            sig.filesize=static_cast<std::uint32_t>(14+40+imagesize);
            sig.save(output);
            dib.save(output);
            
            for (auto row = 0 ; row < height;row++){
                for (auto col = 0 ; col < width;col++){
                    output.write(reinterpret_cast<const char*>(&image.color(height-row-1, col).value),2);
                }
                output.write(paddata.data(),paddata.size());
            }
        }
        return rvalue ;
    }

}
