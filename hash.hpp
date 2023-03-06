//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef hash_hpp
#define hash_hpp

#include <cstdint>
#include <string>
#include <vector>

//=========================================================================================
namespace uo {
    auto hashLittle2(const std::string &hashstring) ->std::uint64_t;
    
    auto hashAdler32(const std::uint8_t *ptr,std::size_t amount) ->std::uint32_t ;
    auto hashAdler32(const std::vector<std::uint8_t> &data) ->std::uint32_t;
    auto hashAdler32(std::iostream &input,std::size_t amount) ->std::uint32_t;

}
#endif /* hash_hpp */
