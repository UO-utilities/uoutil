//Copyright © 2023 Charles Kerr. All rights reserved.

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "general.hpp"

using namespace std::string_literals ;

namespace uo {
    //=========================================================================================
    auto uo_location = std::filesystem::path() ;
    
    std::unordered_map<uotype_t, uoformat_t> data_format{
        {uotype_t::anim,uoformat_t::uop},
        {uotype_t::art,uoformat_t::uop},
        {uotype_t::font,uoformat_t::mul},
        {uotype_t::gump,uoformat_t::uop},
        {uotype_t::hue,uoformat_t::mul},
        {uotype_t::light,uoformat_t::idxmul},
        {uotype_t::map,uoformat_t::uop},
        {uotype_t::multi,uoformat_t::uop},
        {uotype_t::skill,uoformat_t::idxmul},
        {uotype_t::sound,uoformat_t::uop},
        {uotype_t::mapsta,uoformat_t::idxmul},
        {uotype_t::texture,uoformat_t::idxmul},
        {uotype_t::tileinfo,uoformat_t::mul},
        {uotype_t::housing,uoformat_t::bin}
    };
    
    std::unordered_map<uotype_t,std::pair<std::string,std::string>> uo_filenames{
        {uotype_t::anim,{"AnimationFrame%i.uop"s,""s}},
        {uotype_t::art,{"artLegacyMUL.uop"s,""s}},
        {uotype_t::font,{"fonts.mul"s,""s}},
        {uotype_t::gump,{"gumpartLegacyMUL.uop"s,""s}},
        {uotype_t::hue,{"hues.mul"s,""s}},
        {uotype_t::light,{"light.mul"s,"lightidx.mul"s}},
        {uotype_t::map,{"map%iLegacyMUL.uop"s,""s}},
        {uotype_t::multi,{"MultiCollection.uop"s,""s}},
        {uotype_t::skill,{"skills.mul"s,"Skills.idx"s}},
        {uotype_t::sound,{"soundLegacyMUL.uop"s,""s}},
        {uotype_t::mapsta,{"statics%i.mul"s,"staidx%i..mul"s}},
        {uotype_t::texture,{"texmaps.mul"s,"texidx.mul"s}},
        {uotype_t::tileinfo,{"tiledata.mul"s,""s}},
        {uotype_t::housing,{"housing.bin"s,""s}}
    };
    
    std::unordered_map<uotype_t, std::string> uo_hashes{
        {uotype_t::anim,""s},
        {uotype_t::art,"build/artlegacymul/%08u.tga"s},
        {uotype_t::gump,"build/gumpartlegacymul/%08u.tga"s},
        {uotype_t::map,"build/map%ilegacymul/%.8u.dat"s},
        {uotype_t::multi,"build/multicollection/%06u.bin"s},
        {uotype_t::sound,"build/soundlegacymul/%08u.dat"s},
        {uotype_t::housing,"build/multicollection/housing.bin"s}
    };
    //=========================================================================================
    auto locDefaultForUOData() -> const std::filesystem::path& {
        if (uo_location.empty()){
            auto value = std::string(std::getenv("UODIR")) ;
            if (!value.empty()){
                auto temp = std::filesystem::path(value);
                if (std::filesystem::exists(temp)){
                    uo_location = temp ;
                }
            }
        }
        if (uo_location.empty()){
            auto temp  = std::filesystem::path(_UODIR);
            if (std::filesystem::exists(temp)){
                uo_location = temp ;
            }
        }
        return uo_location;
    }
    
}
