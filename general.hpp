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
    /// Defines the different data types for uo
    enum class uotype_t {
        anim,art,font,gump,hue,light,map,multi,skill,sound,mapsta,texture,tileinfo,housing
    };
    /// The three different formats: uop, idx and mul, or just mul
    enum class uoformat_t { uop,mul,idxmul,bin};
    
    /// When dealing with tiles, there are those in maps (terrain), those in statics (art), and thos reference in multi (multi)
    /// The multies are "art", but when talking structure types (not artwork), they have additional information about where they are in the multi
    enum class tiletype_t { none,terrain,art,multi} ;
    
    /// Map that defines the different data formats for each datatype
    extern std::unordered_map<uotype_t, uoformat_t> data_format ;
    
    /// Map that defines the file names for each data type (it is always datafile, then index file). If no index file, that is empty()
    extern std::unordered_map<uotype_t,std::pair<std::string,std::string>> uo_filenames ;
    /// Map that defines the hashformat for each uop data type
    extern std::unordered_map<uotype_t, std::string> uo_hashes ;
    
    /// This will return the uo data directory as defined by the enviroment UODIR.  If that isn't set, then returns
    /// the location defined by the define "_UODIR" ._
    /// That define (if not defined at compile type ) is set to be
    /// Windows:
    ///     "C:\\Program Files (x86)\\Electronic Arts\\Ultima Online Classic"
    /// Unix:
    ///     "~/uodata
    auto locDefaultForUOData() ->const std::filesystem::path & ;
    
    /// A type def for tileid
    using tileid_t = std::uint16_t ;
    /// Invalid tiles have an id of 0xFFFF
    constexpr auto invalid_tile = tileid_t(0xFFFF) ;
    /// The maximum tileid is 0x3fff, so a total of 0x4000 tiles (counting 0)
    constexpr auto terrain_tile_max = tileid_t(0x4000) ;

    //==========================================================
    // The source for this was found on StackOverflow at:
    // https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
    // It was slightly modified for a tad of error checking.
    
    /// Allows one to format using strings as the format specifier
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
