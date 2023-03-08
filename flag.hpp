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
        
        /// The names of the flag masks. Allows one to get a string for a flag mask
        static const std::unordered_map<std::string,std::uint64_t> flagname_masks ;
        /// obtains the mask for a flag name
        /// Parameters:
        /// - flag name: the name of the flag
        /// - Returns: the flag mask
        static auto maskForName(const std::string &flag_name) ->std::uint64_t ;
        /// obtains flag for a specifig flag bit (1-64). NOT 0 based!
        /// Parameters:
        /// - bit: the flag bit (1-64) NOT 0 based!
        /// - Returns: the flag name
        static auto flagNameForBit(int bit) ->const std::string& ;
        /// generates a header of all the flag names, useful for a title row in a csv file
        /// Parameters:
        /// - output: the stream to the text file
        /// - sep: The separator to use between the flag names
       /// - Returns: nothing
        static auto header(std::ostream &output , const std::string &sep=",")  ->void ;

        /// The actual value of all flags
        std::uint64_t value ;
        
        /// Constructor for the flag object
        /// Parameters:
        /// - flag value: The inital value for all flags
        flag_t(std::uint64_t flag_value=0) ;
        /// Constructor for the flag object
        /// Parameters:
        /// - value: a text string of all flag values (1 or blank/0)
        /// - sep: the separtor used between the values ;
        flag_t(const std::string &value,const std::string &sep=":") ;
        /// Constructor for the flag object
        /// Parameters:
        /// - values: vector of string values (0/1 or empty) for each flag
        flag_t(const std::vector<std::string> &values);
        
        /// Determines if any of the flag bits are set in the mask
        /// Parameters:
        /// - mask: the mask to compare to
        /// - Returns: true if any flag bit is set
        auto hasFlag(std::uint64_t mask) const -> bool ;
        /// Sets flag bits are set in the mask
        /// Parameters:
        /// - mask: the mask to "or" with
        /// - Returns: a reference to the flag object
        [[maybe_unused]] auto setFlag(std::uint64_t mask) ->flag_t& ;
        /// Generates a string of flags that are set
        /// Parameters:
        /// - sep: separator to use between names, defaults to ":"
        /// - Returns: a string with the flag names that are set
        auto description(const std::string &sep=":") const ->std::string ;
        
        /// Generates on one line, a 1/empty value for each flag
        /// Individual entries are separated by the specified sep.
        /// Useful when wanting to generating csv files
        /// Parameters:
        /// - output: output stream to the text file
        /// - sep: separator to use between names, defaults to ":"
        /// - Returns: a string with the flag names that are set
       auto column(std::ostream &output , const std::string &sep=",") const ->void ;
        
        /// Allows one to set the flag object with a unsigned 64 bit value
        [[maybe_unused]] auto operator=(std::uint64_t value) ->flag_t& ;
        /// Allows one to set the flag object with a signed 64 bit value
        [[maybe_unused]] auto operator=(std::int64_t value) ->flag_t& ;
        /// Allows one to set the flag object with a unsigned 32 bit value
        [[maybe_unused]] auto operator=(std::uint32_t value) ->flag_t& ;
        /// Allows one to set the flag object with a signed 32 bit value
        [[maybe_unused]] auto operator=(std::int32_t value) ->flag_t& ;
    };
}
#endif /* flag_hpp */
