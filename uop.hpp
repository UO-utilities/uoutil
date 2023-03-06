//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef uop_hpp
#define uop_hpp

#include <cstdint>
#include <string>
#include <vector>
#include <istream>
#include <ostream>
#include <fstream>
#include <filesystem>

//=========================================================================================
namespace uo {
    //=========================================================================================
    // Constants used
    //=========================================================================================
    // Define the version we support
    constexpr   auto uop_version = std::uint32_t(5) ;
    // Define the default table_size
    constexpr   auto default_table_size  = std::uint32_t(1000) ;

    
    //=========================================================================================
    struct uop_table_entry {
        static constexpr auto entry_size = 34 ;
        std::uint64_t    file_offset ;          // Offset into the file where the data associated with the entry is located at
        std::uint32_t    header_length ;        // If that data has a header, the length of it. To offset to the data, you would do: offset + header_length ;
        std::uint32_t    compressed_length ;    // THe size of the compressed data (you use this to read the data)
        std::uint32_t    decompressed_length ;  // The size of the data after decompressing
        std::uint64_t    hash ;                 // What is the hash value for this entry
        std::uint32_t    data_hash ;            // The hash of the actual data (not sure how this is used)
        std::int16_t     use_compression ;      // If 1, then the data is compressed with zlib
        uop_table_entry();
        uop_table_entry(const std::uint8_t *ptr) ;
        uop_table_entry(std::istream &input);
        
        auto read(std::istream &input) ->void ;
        auto read(const std::uint8_t *ptr) ->void ;
        auto save(std::uint8_t *ptr) const ->void ;
        auto save(std::ostream &input) const ->void ;
        auto data() const ->std::vector<std::uint8_t> ;
        auto valid() const ->bool ;
    };
    
    //=========================================================================================
    struct uop_table_header {
        static constexpr auto entry_length = 12 ;
        
        std::uint32_t number_entries ;
        std::uint64_t next_table ;
        uop_table_header():number_entries(0),next_table(0){}
        uop_table_header(const std::uint8_t *ptr) ;
        uop_table_header(std::istream &input);
        auto load(const std::uint8_t *ptr) ->void ;
        auto load(std::istream &input) ->void ;
        auto save(std::ostream &output) const ->void ;
    
    };
    //=========================================================================================
    class uop_info {
        const std::uint8_t *uop_ptr ;
        std::ifstream input ;
        
        bool valid_uop ;
        std::uint32_t entry_count ;
        std::uint64_t table_start ;
        std::uint32_t max_table_size ;

        std::vector<uop_table_entry> table ;

        auto initialize() ->void ;
        auto loadTable() ->void ;
        
    public:
        uop_info() ;
        uop_info(const std::uint8_t *ptr ) ;
        uop_info(const std::filesystem::path &path);
        auto valid() const ->bool ;
        auto size() const ->std::uint32_t ;
        auto maxTableSize() const ->std::uint32_t ;
        auto firstTable() const ->std::uint64_t ;
        auto streamsrc() ->std::ifstream & ;
        auto memsrc() ->const std::uint8_t *  ;
        auto operator[](std::uint32_t index) const ->const uop_table_entry& ;
        auto operator[](std::uint32_t index) ->uop_table_entry& ;
    };

    //=========================================================================================
    auto validUOP(const std::uint8_t *ptr) ->bool ;
    auto validUOP(std::istream &input) ->bool ;
    
    
    auto compressData(const std::vector<std::uint8_t> &data) ->std::vector<std::uint8_t>;
    auto compressData(const std::uint8_t *data,std::size_t size) ->std::vector<std::uint8_t>;
    
    auto createUOP(std::ostream &output, std::uint32_t numentries,std::uint32_t maxtablesize=default_table_size,std::uint32_t version = uop_version ) ->std::vector<std::uint64_t> ;
    
    auto readUOPData(const uop_table_entry &entry, std::istream &input) ->std::vector<std::uint8_t>;
    auto readUOPData(const uop_table_entry &entry, const std::uint8_t *ptr) ->std::vector<std::uint8_t>;
}
#endif /* uop_hpp */
