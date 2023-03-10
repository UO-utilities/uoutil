//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef multi_hpp
#define multi_hpp

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <istream>
#include <ostream>
#include <filesystem>

#include "general.hpp"
#include "flag.hpp"
//=========================================================================================
namespace uo {
    
    
    //==================================================================================
    struct multi_item{
        static const std::string header_line ;
        static constexpr auto min_size = size_t(14) ;
        static constexpr auto mul_record_size = size_t(16);
        
        tileid_t tileid ;
        std::int16_t offsetx ;
        std::int16_t offsety ;
        std::int16_t offsetz ;
        flag_t flag ;
        std::vector<std::uint32_t> cliloc ;
        
        multi_item() ;
        multi_item(const std::string &linevalue);
        auto load(const std::uint8_t *data,bool has_cliloc) ->size_t ;
        auto description(bool use_hex) const ->std::string;
        auto data(bool with_clloc) const ->std::vector<std::uint8_t> ;
    };
    
    //==================================================================================
    struct multi_t {
        static const std::map<std::uint32_t,std::string> names ;
        std::vector<multi_item> items ;
        std::int16_t minx ;
        std::int16_t miny ;
        std::int16_t minz ;
        std::int16_t maxx ;
        std::int16_t maxy ;
        std::int16_t maxz ;
        multi_t() :minz(std::numeric_limits<std::int16_t>::max()),maxz(std::numeric_limits<std::int16_t>::min()),miny(std::numeric_limits<std::int16_t>::max()),maxy(std::numeric_limits<std::int16_t>::min()),minx(std::numeric_limits<std::int16_t>::max()),maxx(std::numeric_limits<std::int16_t>::min()){}
        multi_t(const std::vector<std::uint8_t> &data, bool has_cliloc);
        auto data(bool has_cliloc) const ->std::vector<std::uint8_t> ;
        auto load(const std::vector<std::uint8_t> &data, bool has_cliloc) ->void ;
        auto load(const std::filesystem::path &path) ->void ;
        auto load(std::istream &input)->void ;
        auto description(const std::filesystem::path &path,bool use_hex) ->void ;
        auto description(std::ostream &output,bool use_hex) ->void;

    };

    //==================================================================================
    // Routines
    //==================================================================================

    auto hashForMulti(std::uint32_t id) ->std::uint64_t ;
    auto hashForHousing() ->std::uint64_t ;

    
}
#endif /* multi_hpp */
