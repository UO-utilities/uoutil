//Copyright © 2023 Charles Kerr. All rights reserved.

#include <iostream>
#include <stdexcept>

#include "idx.hpp"

using namespace std::string_literals ;

//=========================================================================================
namespace uo {
    //=========================================================================================
    idx_entry::idx_entry(std::uint32_t file_offset,std::uint32_t data_length ,std::uint32_t extra_data ):offset(file_offset),length(data_length),extra(extra_data){
    }
    //=========================================================================================
    idx_entry::idx_entry(std::istream &input):idx_entry(){
        read(input) ;
    }
    
    //=========================================================================================
    auto  idx_entry::valid() const ->bool {
        return length < std::numeric_limits<std::uint32_t>::max();
    }
    //=========================================================================================
    auto  idx_entry::tex64() const ->bool {
        return extra == 0 ;
    }
    //=========================================================================================
    auto  idx_entry::tex128() const ->bool {
        return extra == 1;
    }
    //=========================================================================================
    auto  idx_entry::lightWidth() const ->std::uint16_t {
        return static_cast<std::uint16_t>((extra&0xFF));
   }
    //=========================================================================================
    auto  idx_entry::lightHeight() const ->std::uint16_t{
        return static_cast<std::uint16_t>(((extra>>16)&0xFF));

    }
    //=========================================================================================
    auto  idx_entry::save(std::ostream &output) const ->void {
        output.write(reinterpret_cast<const char*>(&offset),sizeof(offset));
        output.write(reinterpret_cast<const char*>(&length),sizeof(length));
        output.write(reinterpret_cast<const char*>(&extra),sizeof(extra));

    }
    //=========================================================================================
    auto  idx_entry::read(std::istream &input) ->void {
        input.read(reinterpret_cast<char*>(&offset),sizeof(offset));
        input.read(reinterpret_cast<char*>(&length),sizeof(length));
        input.read(reinterpret_cast<char*>(&extra),sizeof(extra));
    }

}
