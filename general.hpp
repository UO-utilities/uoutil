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

    //==========================================================
    // The source for this was found on StackOverflow at:
    // https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
    // It was slightly modified for a tad of error checking.
    template <typename... Args>
    std::string format(const std::string &format_str, Args... args) {
        auto rvalue = std::string();
        if (!format_str.empty()) {

            // First see how much space we need?
            auto size_s = std::snprintf(nullptr, 0, format_str.c_str(), args...);
            if (size_s < 0) {
                throw std::runtime_error("Error applying format string");
            }
            if (size_s > 0) {
                // Take the space we need and add 1 for the terminating \0
                size_s += 1;
                auto size = static_cast<std::size_t>(size_s);
                // Lets create a buffer we need for the data
                auto buf = std::make_unique<char[]>(size);
                size_s =
                std::snprintf(buf.get(), size, format_str.c_str(), args...);
                if (size_s < 0) {
                    throw std::runtime_error("Error applying format string");
                }
                if (size_s > 0) {
                    rvalue = std::string(buf.get(), buf.get() + size_s);
                }
            }
         }
        return rvalue;
    }

}
#endif /* general_hpp */
