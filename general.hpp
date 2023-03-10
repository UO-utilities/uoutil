//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef general_hpp
#define general_hpp

#include <cstdint>
#include <string>
#include <filesystem>
#include <utility>
#include <unordered_map>
#include <vector>
#include <cstdio>

//=========================================================================================
#if !defined(_UODIR)
#if defined(_WIN32)
#define _UODIR "C:\\Program Files (x86)\\Electronic Arts\\Ultima Online Classic"
#else
#define _UODIR "~/uodata"
#endif
#endif

namespace uo {
    
    /// Define the version number of the library
    constexpr auto version = 0 ;
    /// Defines the different data types for uo
    enum class uotype_t {
        none,anim,art,font,gump,hue,light,map,multi,skill,sound,mapsta,texture,tileinfo,housing
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

    /// The id type for hues
    using hueid_t = std::uint16_t ;
    
    
    //==================================================================================
    // The following are lifted from our utility library, strutil namespace
    //==================================================================================

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

    //=========================================================
    // Trim all whitespace from the left of the string
    inline auto ltrim(const std::string &value) -> std::string {
        if (!value.empty()) {
            auto loc = value.find_first_not_of(" \t\v\f\n\r");
            if (loc == std::string::npos) {
                return std::string();
            }
            return value.substr(loc);
        }
        return value;
    }
    //=========================================================
    // Trim all whitespace from the right of the string
    inline auto rtrim(const std::string &value) -> std::string {
        if (!value.empty()) {
            auto loc = value.find_last_not_of(" \t\v\f\n\r");
            if (loc == std::string::npos) {
                return value;
            }
            return value.substr(0, loc + 1);
        }
        return value;
    }
    //=========================================================
    // Trim all whitespace from both sides of the string
    inline auto trim(const std::string &value) -> std::string {
        return rtrim(ltrim(value));
    }
    //=========================================================
    inline auto parse(const std::string &value, const std::string &sep) -> std::vector<std::string> {
        std::vector<std::string> rvalue;
        auto current = std::string::size_type(0);
        if (!sep.empty()){
            auto loc = value.find(sep, current);
            while (loc != std::string::npos) {
                rvalue.push_back(trim(value.substr(current, loc - current)));
                current = loc + sep.size();
                if (current >= value.size()){
                    rvalue.push_back("");
                }
                loc = value.find(sep, current);
            }
            if (current < value.size()) {
                rvalue.push_back(trim(value.substr(current)));
            }
        }
        else {
            rvalue.push_back(trim(value));
        }
        return rvalue;
    }
    //=========================================================
    inline auto strip(const std::string &value, const std::string &sep = "//", bool pack = true) -> std::string {
        auto rvalue = value;
        if (!sep.empty()){
            auto loc = rvalue.find(sep);
            if (loc != std::string::npos) {
                rvalue = rvalue.substr(0, loc);
            }
            if (pack) {
                rvalue = rtrim(rvalue);
            }
        }
        return rvalue;
    }

}
#endif /* general_hpp */
