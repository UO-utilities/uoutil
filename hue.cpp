//Copyright © 2023 Charles Kerr. All rights reserved.

#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "hue.hpp"
#include "color.hpp"
using namespace std::string_literals ;

//=========================================================================================
namespace uo {
    constexpr auto hues_in_group = 8 ;
    constexpr auto hue_entry_size = 88 ;
    //=========================================================================================
    auto offsetForHue(hueid_t hueid) ->std::uint64_t{
        auto header_count = static_cast<int>(hueid)/hues_in_group ;
        return static_cast<std::uint64_t>(((header_count+1)*4) + hueid*hue_entry_size) ;
    }
    
    //=========================================================================================
    auto colorsForHue(std::istream &input,hueid_t hueid) ->std::vector<color_t> {
        auto offset = offsetForHue(hueid) ;
        auto rvalue = std::vector<color_t>();
        input.seekg(offset,std::ios::beg) ;
        if (!input.good()){
            throw std::runtime_error("Hue id exceeds contents of stream");
        }
        auto color = std::uint16_t(0) ;
        for (auto j=0 ; j< 32;j++){
            input.read(reinterpret_cast<char*>(&color),2);
            if (input.gcount()!= 2){
                throw std::runtime_error("Insufficient data in stream for hueid "s + std::to_string(hueid));
            }
            rvalue.push_back(color_t(color));
        }
        return rvalue ;
    }
    
    //=========================================================================================
    auto colorsForHue(const std::uint8_t *ptr,std::size_t size,hueid_t hueid) ->std::vector<color_t> {
        auto offset = offsetForHue(hueid) ;
        if (offset + hue_entry_size > size){
            throw std::runtime_error("Hue id exceeds contents of data");
        }
        auto rvalue = std::vector<color_t>();
        auto data = reinterpret_cast<const std::uint16_t*>(ptr+offset);
        for (auto j=0 ; j< 32;j++){
            rvalue.push_back(color_t(*(data+j)));
        }
        return rvalue ;
    }
    
    //=========================================================================================
    auto setColorsForHue(std::ostream &output,hueid_t hueid,const std::vector<color_t> &colors) ->void {
        auto count = std::min(colors.size(),static_cast<std::size_t>(32));
        auto offset = offsetForHue(hueid);
        output.seekp(offset,std::ios::beg);
        if (!output.good()){
            throw std::runtime_error("Insufficent size in stream for hueid");
        }
        for (std::size_t j=0 ; j< count;j++){
            output.write(reinterpret_cast<const char*>(&colors.at(j).value),2);
            if (!output.good()){
                throw std::runtime_error("Insufficent size in stream for hueid");
            }
        }
        output.seekp(offset+64,std::ios::beg);
        // now we need to write the first/last of the table
        if (count>0){
            output.write(reinterpret_cast<const char*>(&colors.at(0).value),2);
        }
        if (count ==32){
            output.write(reinterpret_cast<const char*>(&colors.at(31).value),2);
        }
    }
    
    //=========================================================================================
    auto setColorsForHue(std::uint8_t *ptr , std::size_t size, hueid_t hueid,const std::vector<color_t> &colors) ->void {
        auto count = std::min(colors.size(),static_cast<std::size_t>(32));
        auto offset = offsetForHue(hueid);
        if ((offset+hue_entry_size) > size) {
            throw std::runtime_error("Insufficent size in data for hueid");
        }
        for (std::size_t j=0;j<count;j++){
            std::copy(reinterpret_cast<const std::uint8_t*>(&colors.at(j).value),reinterpret_cast<const std::uint8_t*>(&colors.at(j).value)+2, ptr+offset+j*2);
        }
        if (count>0){
            std::copy(reinterpret_cast<const std::uint8_t*>(&colors.at(0).value),reinterpret_cast<const std::uint8_t*>(&colors.at(0).value)+2, ptr+offset+64);
        }
        if (count ==32){
            std::copy(reinterpret_cast<const std::uint8_t*>(&colors.at(31).value),reinterpret_cast<const std::uint8_t*>(&colors.at(31).value)+2, ptr+offset+66);
        }

    }

    //=========================================================================================
    auto stringForHue(std::istream &input,hueid_t hueid) ->std::string {
        auto offset = offsetForHue(hueid) + (32*2)+4;
        input.seekg(offset,std::ios::beg) ;
        if (!input.good()){
            throw std::runtime_error("Hue id exceeds contents of stream");
        }
        auto buffer = std::vector<char>(21,0) ;
        input.read(buffer.data(),20) ;
        if (input.gcount()!=20){
            throw std::runtime_error("Insufficient data in stream for hueid "s + std::to_string(hueid));
        }
        std::string rvalue = buffer.data();
        return rvalue ;
    }
    //=========================================================================================
    auto stringForHue(const std::uint8_t *ptr,std::size_t size,hueid_t hueid) ->std::string {
        auto offset = offsetForHue(hueid) + (32*2)+4;
        if ((offset+20)>size) {
            throw std::runtime_error("Insufficient data for hueid "s + std::to_string(hueid));
        }
        auto buffer = std::vector<char>(21,0) ;

        std::copy(ptr+offset,ptr+offset+20,reinterpret_cast<std::uint8_t*>(buffer.data()));
        std::string rvalue = buffer.data();
        return rvalue ;
    }

    //=========================================================================================
    auto setStringForHue(std::ostream &output,hueid_t hueid,const std::string &text) ->void{
        auto offset = offsetForHue(hueid) + (32*2)+4;
        output.seekp(offset,std::ios::beg);
        if (!output.good()){
            throw std::runtime_error("Hue id exceeds contents of stream");
        }
        auto length = std::min(std::size_t(20),text.size());
        output.write(text.c_str(),length);
        if (length < 20){
            auto buffer = std::vector<char>(20-length,0);
            output.write(buffer.data(),buffer.size());
        }
    }
    //=========================================================================================
    auto setStringForHue(std::uint8_t *ptr,std::size_t size, hueid_t hueid,const std::string &text) ->void{
        auto offset = offsetForHue(hueid) + (32*2)+4;
        if (offset+20 > size){
            throw std::runtime_error("Hue id exceeds contents of data");
        }
        auto length = std::min(std::size_t(20),text.size());
        std::copy(text.c_str(),text.c_str()+length,reinterpret_cast<char*>(ptr)+offset);
        if (length < 20){
            for (auto j=0 ; j< 20-length;j++){
                *(ptr+offset+length+j) = 0 ;
            }
        }
    }
}
