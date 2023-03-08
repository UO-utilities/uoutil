//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef idx_hpp
#define idx_hpp

#include <cstdint>
#include <string>
#include <limits>
//=========================================================================================
namespace uo {
    
    
    struct idx_entry{
        std::uint32_t offset ;
        std::uint32_t length ;
        std::uint32_t extra ;
        
        idx_entry(std::uint32_t file_offset = 0,std::uint32_t data_length = std::numeric_limits<std::uint32_t>::max(),std::uint32_t extra_data = 0);
        idx_entry(std::istream &input);
        
        auto valid() const ->bool ;
        auto tex64() const ->bool ;
        auto tex128() const ->bool ;
        auto lightWidth() const ->std::uint16_t ;
        auto lightHeight() const ->std::uint16_t ;
        auto save(std::ostream &output) const ->void ;
        auto read(std::istream &input) ->void ;
    };
}
#endif /* idx_hpp */
