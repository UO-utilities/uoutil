//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef general_hpp
#define general_hpp

#include <cstdint>
#include <string>
#include <filesystem>
#include <utility>
#include <unordered_map>

//=========================================================================================
#if !defined(_UODIR)
#if defined(_WIN32)
#define _UODIR "C:\\Program Files (x86)\\Electronic Arts\\Ultima Online Classic"
#else
#define _UODIR "~/uodata"
#endif
#endif

namespace uo {
    enum class uotype_t {
        anim,art,font,gump,hue,light,map,multi,skill,sound,mapsta,texture,tileinfo,housing
    };
    enum class uoformat_t { uop,mul,idxmul,bin};
    
    enum class tiletype_t { none,terrain,art,multi} ;
    
    extern std::unordered_map<uotype_t, uoformat_t> data_format ;
    extern std::unordered_map<uotype_t,std::pair<std::string,std::string>> uo_filenames ;
    extern std::unordered_map<uotype_t, std::string> uo_hashes ;
    
    auto locDefaultForUOData() ->const std::filesystem::path & ;
    
    using tileid_t = std::uint16_t ;
    constexpr auto invalid_tile = tileid_t(0xFFFF) ;
    constexpr auto terrain_tile_max = tileid_t(0x4000) ;
    
}
#endif /* general_hpp */
