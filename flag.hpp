//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef flag_hpp
#define flag_hpp

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

//=========================================================================================
namespace uo {
    struct flag_t{
        constexpr static auto   background=0x1ull;
        constexpr static auto   weapon=0x2ull;
        constexpr static auto   transparent=0x4ull;
        constexpr static auto   translucent=0x8ull;

        constexpr static auto   wall=0x10ull;
        constexpr static auto   damaging=0x20ull;
        constexpr static auto   impassable=0x40ull;
        constexpr static auto   wet=0x80ull;

        constexpr static auto   bit9=0x100ull;
        constexpr static auto   surface=0x200ull;
        constexpr static auto   climbable=0x400ull;
        constexpr static auto   stackable=0x800ull;

        constexpr static auto   window=0x1000ull;
        constexpr static auto   noshoot=0x2000ull;
        constexpr static auto   articlea=0x4000ull;
        constexpr static auto   articlean=0x8000ull;

        constexpr static auto   articlethe=0x10000ull;
        constexpr static auto   foliage=0x20000ull;
        constexpr static auto   partialhue=0x40000ull;
        constexpr static auto   nohouse=0x80000ull;

        constexpr static auto   map=0x100000ull;
        constexpr static auto   container=0x200000ull;
        constexpr static auto   wearable=0x400000ull;
        constexpr static auto   lightsource=0x800000ull;

        constexpr static auto   animation=0x1000000ull;
        constexpr static auto   hoverover=0x2000000ull;
        constexpr static auto   nodiagnol=0x4000000ull;
        constexpr static auto   armor=0x8000000ull;

        constexpr static auto   roof=0x10000000ull;
        constexpr static auto   door=0x20000000ull;
        constexpr static auto   stairback=0x40000000ull;
        constexpr static auto   stairright=0x80000000ull;

        constexpr static auto   alphablend=0x100000000ull;
        constexpr static auto   usenewart=0x200000000ull;
        constexpr static auto   artused=0x400000000ull;
        constexpr static auto   bit36=0x800000000ull;

        constexpr static auto   noshadow=0x1000000000ull;
        constexpr static auto   pixelbleed=0x2000000000ull;
        constexpr static auto   playanimonce=0x4000000000ull;
        constexpr static auto   bit40=0x8000000000ull;

        constexpr static auto   multimovable=0x10000000000ull;
        constexpr static auto   bit42=0x20000000000ull;
        constexpr static auto   bit43=0x40000000000ull;
        constexpr static auto   bit44=0x80000000000ull;

        constexpr static auto   bit45=0x100000000000ull;
        constexpr static auto   bit46=0x200000000000ull;
        constexpr static auto   bit47=0x400000000000ull;
        constexpr static auto   bit48=0x800000000000ull;

        constexpr static auto   bit49=0x1000000000000ull;
        constexpr static auto   bit50=0x2000000000000ull;
        constexpr static auto   bit51=0x4000000000000ull;
        constexpr static auto   bit52=0x8000000000000ull;

        constexpr static auto   bit53=0x10000000000000ull;
        constexpr static auto   bit54=0x20000000000000ull;
        constexpr static auto   bit55=0x40000000000000ull;
        constexpr static auto   bit56=0x80000000000000ull;

        constexpr static auto   bit57=0x100000000000000ull;
        constexpr static auto   bit58=0x200000000000000ull;
        constexpr static auto   bit59=0x400000000000000ull;
        constexpr static auto   bit60=0x800000000000000ull;

        constexpr static auto   bit61=0x1000000000000000ull;
        constexpr static auto   bit62=0x2000000000000000ull;
        constexpr static auto   bit63=0x4000000000000000ull;
        constexpr static auto   bit64=0x8000000000000000ull;
        
        static const std::unordered_map<std::string,std::uint64_t> flagname_masks ;
        static auto maskForName(const std::string &flag_name) ->std::uint64_t ;
        static auto flagNameForBit(int bit) ->const std::string& ;
        static auto header(std::ostream &output , const std::string &sep=",")  ->void ;

        std::uint64_t value ;
        
        flag_t(std::uint64_t flag_value=0) ;
        flag_t(const std::string &value,const std::string &sep=":") ;
        flag_t(const std::vector<std::string> &values);
        
        auto hasFlag(std::uint64_t mask) const -> bool ;
        [[maybe_unused]] auto setFlag(std::uint64_t mask) ->flag_t& ;
        auto description(const std::string &sep=":") const ->std::string ;
        auto column(std::ostream &output , const std::string &sep=",") const ->void ;
        [[maybe_unused]] auto operator=(std::uint64_t value) ->flag_t& ;
        [[maybe_unused]] auto operator=(std::int64_t value) ->flag_t& ;
        [[maybe_unused]] auto operator=(std::uint32_t value) ->flag_t& ;
        [[maybe_unused]] auto operator=(std::int32_t value) ->flag_t& ;
    };
}
#endif /* flag_hpp */
