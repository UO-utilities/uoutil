//Copyright © 2023 Charles Kerr. All rights reserved.

#include <iostream>
#include <stdexcept>

#include "flag.hpp"

using namespace std::string_literals ;

//=========================================================================================
namespace uo {
    //=================================================================================
    // Define flagnames
    //=================================================================================
    const std::unordered_map<std::string,std::uint64_t> flag_t::flagname_masks{
        {"background"s,flag_t::background},{"weapon"s,flag_t::weapon},{"transparent"s,flag_t::transparent},{"translucent"s,flag_t::translucent},
        {"wall"s,flag_t::wall},{"damaging"s,flag_t::damaging},{"impassable"s,flag_t::impassable},{"wet"s,flag_t::wet},
        {"bit9"s,flag_t::bit9},{"surface"s,flag_t::surface},{"climbable"s,flag_t::climbable},{"stackable"s,flag_t::stackable},
        {"window"s,flag_t::window},{"noshoot"s,flag_t::noshoot},{"articlea"s,flag_t::articlea},{"articlean"s,flag_t::articlean},
        {"articlethe"s,flag_t::articlethe},{"foliage"s,flag_t::foliage},{"partialhue"s,flag_t::partialhue},{"nohouse"s,flag_t::nohouse},
        {"map"s,flag_t::map},{"container"s,flag_t::container},{"wearable"s,flag_t::wearable},{"lightsource"s,flag_t::lightsource},
        {"animation"s,flag_t::animation},{"hoverover"s,flag_t::hoverover},{"nodiagnol"s,flag_t::nodiagnol},{"armor"s,flag_t::armor},
        {"roof"s,flag_t::roof},{"door"s,flag_t::door},{"stairback"s,flag_t::stairback},{"stairright"s,flag_t::stairright},
        {"alphablend"s,flag_t::alphablend},{"usenewart"s,flag_t::usenewart},{"artused"s,flag_t::artused},{"bit36"s,flag_t::bit36},
        {"noshadow"s,flag_t::noshadow},{"pixelbleed"s,flag_t::pixelbleed},{"playanimonce"s,flag_t::playanimonce},{"bit40"s,flag_t::bit40},
        {"multimovable"s,flag_t::multimovable},{"bit42"s,flag_t::bit42},{"bit43"s,flag_t::bit43},{"bit44"s,flag_t::bit44},
        {"bit45"s,flag_t::bit45},{"bit46"s,flag_t::bit46},{"bit47"s,flag_t::bit47},{"bit48"s,flag_t::bit48},
        {"bit49"s,flag_t::bit49},{"bit50"s,flag_t::bit50},{"bit51"s,flag_t::bit51},{"bit52"s,flag_t::bit52},
        {"bit53"s,flag_t::bit53},{"bit54"s,flag_t::bit54},{"bit55"s,flag_t::bit55},{"bit56"s,flag_t::bit56},
        {"bit57"s,flag_t::bit57},{"bit58"s,flag_t::bit58},{"bit59"s,flag_t::bit59},{"bit60"s,flag_t::bit60},
        {"bit61"s,flag_t::bit61},{"bit62"s,flag_t::bit62},{"bit63"s,flag_t::bit63},{"bit64"s,flag_t::bit64}
    };
    //=================================================================================
    // Define flag_t::maskForName
    //=================================================================================
    //=================================================================================
    auto flag_t::maskForName(const std::string &flag_name) ->std::uint64_t {
        auto iter = flagname_masks.find(flag_name) ;
        if (iter != flagname_masks.end()){
            return iter->second ;
        }
        throw std::out_of_range("Flag name: "s+flag_name+" not valid"s);
    }
    
    //=================================================================================
    // Define flag_t::flagNameForBit
    //=================================================================================
    //=================================================================================
    auto flag_t::flagNameForBit(int bit) ->const std::string& {
        auto mask = std::uint64_t(1) ;
        mask = mask<<(bit-1) ;
        auto iter = std::find_if(flagname_masks.begin(),flagname_masks.end(),[mask](const std::pair<std::string ,std::uint64_t> &value){
            return mask == std::get<1>(value);
        });
        if (iter != flagname_masks.end()){
            return iter->first;
        }
        throw std::out_of_range("Bit value not between 1 and 64 inclusive: "s + std::to_string(bit));
    }
    //=================================================================================
    // outputs a header for each flag value
    //=================================================================================
    //=================================================================================
    auto flag_t::header(std::ostream &output , const std::string &sep)  ->void{
        for (auto j=1;j<=64;j++){
            output <<flagNameForBit(j);
            if (j!= 64){
                output<<sep;
            }
        }
    }

    //=================================================================================
    // Constructor
    //=================================================================================
    //=================================================================================
    flag_t::flag_t(std::uint64_t flag_value):value(flag_value){}
    
    //=================================================================================
    // Constructor
    //=================================================================================
    //=================================================================================
    flag_t::flag_t(const std::string &value,const std::string &sep):flag_t() {
        if (!value.empty()){
            auto data = value ;
            do {
                
                auto end = data.find_first_of(sep);
                if (end != std::string::npos){
                    auto svalue = data.substr(0,end) ;
                    data = data.substr(end+sep.size()) ;
                    this->value |= maskForName(svalue) ;
                }
                else {
                    this->value |= maskForName(data) ;
                    data = "";
                }
            }
            while (!data.empty());
        }
    }
    //=================================================================================
    // Constructor
    //=================================================================================
    //=================================================================================
    flag_t::flag_t(const std::vector<std::string> &values):flag_t(){
        auto bit = 0 ;
        auto mask = std::uint64_t(1) ;
        for (const auto &entry:values){
            auto value = false ;
            if (!entry.empty()){
                value = static_cast<bool>(std::stoi(entry)) ;
            }
            if (value){
                this->value|= (mask<<bit);
            }
            bit++;
        }
    }

    //=================================================================================
    // check if the value has the mask.
    //=================================================================================
    //=================================================================================
    auto flag_t::hasFlag(std::uint64_t mask) const -> bool {
        return mask&value ;
    }
    //=================================================================================
    // Set the mask in the flag.
    //=================================================================================
    //=================================================================================
    auto flag_t::setFlag(std::uint64_t mask) ->flag_t& {
        value |= mask ;
        return *this;
    }
    //=================================================================================
    // Text representation
    //=================================================================================
    //=================================================================================
    auto flag_t::description(const std::string &sep) const ->std::string {
       
        auto bit = std::uint64_t(1) ;
        auto text = std::string() ;
        for (auto j=0;j<64;j++) {
            auto mask = bit<<j ;
            if (mask & this->value){
                if (!text.empty()){
                    text += sep ;
                }
                text += flagNameForBit(j+1);
            }
        }
        return text ;
    }
    //=================================================================================
    // outputs a entry for each flag value
    //=================================================================================
    //=================================================================================
    auto flag_t::column(std::ostream &output , const std::string &sep) const ->void{
        auto mask = std::uint64_t(0) ;
        for (auto j=0;j<64;j++){
            if (value&(mask<<j)){
                output <<1;
            }
            if (j<(64-1)){
                output<<sep;
            }
        }
    }
    //=================================================================================
    // operator =
    //=================================================================================
    //=================================================================================
    auto flag_t::operator=(std::uint64_t value) ->flag_t& {
        this->value = value;
        return *this;
    }
    //=================================================================================
    auto flag_t::operator=(std::int64_t value) ->flag_t& {
        this->value = static_cast<std::uint64_t>(value);
        return *this;
    }
    //=================================================================================
    auto flag_t::operator=(std::uint32_t value) ->flag_t& {
        this->value = static_cast<std::uint64_t>(value);
        return *this;
    }
    //=================================================================================
    auto flag_t::operator=(std::int32_t value) ->flag_t& {
        this->value = static_cast<std::uint64_t>(value);
        return *this;
    }

}
