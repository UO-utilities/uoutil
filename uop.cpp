//Copyright © 2023 Charles Kerr. All rights reserved.

#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "uop.hpp"
#include <zlib.h>
using namespace std::string_literals ;

//=========================================================================================
namespace uo {
    
    //=========================================================================================
    // Constants used
    //=========================================================================================
    
    // Define the uop signature 
    constexpr   auto uop_identifer = std::uint32_t(0x50594D);
    
    // Define where the start table offset is define
    //constexpr   auto table_offset_location = std::uint32_t(12) ;
    
    // Define where the tables start for writing purposes
    constexpr   auto starting_offset = std::uint64_t(512) ;
    
    
    
    
    //=========================================================================================
    // internal structures
    //=========================================================================================
    
    
    
    //=========================================================================================
    // uop_table_entry
    //=========================================================================================
    
    //=========================================================================================
    uop_table_entry::uop_table_entry():file_offset(0),header_length(0),compressed_length(0),decompressed_length(0),hash(0),data_hash(0),use_compression(0){
    }
    //=========================================================================================
    uop_table_entry::uop_table_entry(const std::uint8_t *ptr):uop_table_entry() {
        if (ptr == nullptr){
            throw std::runtime_error("uop_table_entry initialization failed: ptr was null");
        }
        this->read(ptr) ;
    }
    //=========================================================================================
    uop_table_entry::uop_table_entry(std::istream &input):uop_table_entry() {
        this->read(input) ;
    }
    //=========================================================================================
    auto uop_table_entry::read(std::istream &input) ->void {
        input.read(reinterpret_cast<char*>(&file_offset),sizeof(file_offset));
        input.read(reinterpret_cast<char*>(&header_length),sizeof(header_length));
        input.read(reinterpret_cast<char*>(&compressed_length),sizeof(compressed_length));
        input.read(reinterpret_cast<char*>(&decompressed_length),sizeof(decompressed_length));
        input.read(reinterpret_cast<char*>(&hash),sizeof(hash));
        input.read(reinterpret_cast<char*>(&data_hash),sizeof(data_hash));
        input.read(reinterpret_cast<char*>(&use_compression),sizeof(use_compression));
    }
    //=========================================================================================
    auto uop_table_entry::read(const std::uint8_t *ptr) ->void{
        auto offset = 0 ;
        
        std::copy(ptr,ptr+sizeof(file_offset),reinterpret_cast<std::uint8_t*>(&file_offset));
        offset+=sizeof(file_offset);
        
        std::copy(ptr+offset,ptr+offset+sizeof(header_length),reinterpret_cast<std::uint8_t*>(&header_length));
        offset+=sizeof(header_length);
        
        std::copy(ptr+offset,ptr+offset+sizeof(compressed_length),reinterpret_cast<std::uint8_t*>(&compressed_length));
        offset+=sizeof(compressed_length);
        
        std::copy(ptr+offset,ptr+offset+sizeof(decompressed_length),reinterpret_cast<std::uint8_t*>(&decompressed_length));
        offset+=sizeof(decompressed_length);
        
        std::copy(ptr+offset,ptr+offset+sizeof(hash),reinterpret_cast<std::uint8_t*>(&hash));
        offset+=sizeof(hash);
        
        std::copy(ptr+offset,ptr+offset+sizeof(data_hash),reinterpret_cast<std::uint8_t*>(&data_hash));
        offset+=sizeof(data_hash);
        
        std::copy(ptr+offset,ptr+offset+sizeof(use_compression),reinterpret_cast<std::uint8_t*>(&use_compression));
    }
    //=========================================================================================
    auto uop_table_entry::save(std::ostream &output) const ->void{
        output.write(reinterpret_cast<const char*>(&file_offset),sizeof(file_offset));
        output.write(reinterpret_cast<const char*>(&header_length),sizeof(header_length));
        output.write(reinterpret_cast<const char*>(&compressed_length),sizeof(compressed_length));
        output.write(reinterpret_cast<const char*>(&decompressed_length),sizeof(decompressed_length));
        output.write(reinterpret_cast<const char*>(&hash),sizeof(hash));
        output.write(reinterpret_cast<const char*>(&data_hash),sizeof(data_hash));
        output.write(reinterpret_cast<const char*>(&use_compression),sizeof(use_compression));
    }
   //=========================================================================================
    auto uop_table_entry::save(std::uint8_t *ptr) const ->void{
        auto offset = 0 ;
        
        std::copy(reinterpret_cast<const std::uint8_t*>(&file_offset),reinterpret_cast<const std::uint8_t*>(&file_offset)+sizeof(file_offset),ptr+offset);
        offset+=sizeof(file_offset);
        
        std::copy(reinterpret_cast<const std::uint8_t*>(&header_length),reinterpret_cast<const std::uint8_t*>(&header_length)+sizeof(header_length),ptr+offset);
        offset+=sizeof(header_length);
        
        std::copy(reinterpret_cast<const std::uint8_t*>(&compressed_length),reinterpret_cast<const std::uint8_t*>(&compressed_length)+sizeof(compressed_length),ptr+offset);
        offset+=sizeof(compressed_length);
        
        std::copy(reinterpret_cast<const std::uint8_t*>(&decompressed_length),reinterpret_cast<const std::uint8_t*>(&decompressed_length)+sizeof(decompressed_length),ptr+offset);
        offset+=sizeof(decompressed_length);
        
        std::copy(reinterpret_cast<const std::uint8_t*>(&hash),reinterpret_cast<const std::uint8_t*>(&hash)+sizeof(hash),ptr+offset);
        offset+=sizeof(hash);
        
        std::copy(reinterpret_cast<const std::uint8_t*>(&data_hash),reinterpret_cast<const std::uint8_t*>(&data_hash)+sizeof(data_hash),ptr+offset);
        offset+=sizeof(data_hash);
        
        std::copy(reinterpret_cast<const std::uint8_t*>(&use_compression),reinterpret_cast<const std::uint8_t*>(&use_compression)+sizeof(use_compression),ptr+offset);
        
    }
    
    //=========================================================================================
    auto uop_table_entry::data() const ->std::vector<std::uint8_t> {
        auto rvalue = std::vector<std::uint8_t>(entry_size,0);
        this->save(rvalue.data());
        return rvalue ;
    }
    //=========================================================================================
    auto uop_table_entry::valid() const ->bool{
        return (hash!=0) && (decompressed_length!=0) ;
    }
    
    
    //=========================================================================================
    // uop_table_header
    //=========================================================================================
    
    //=========================================================================================
    uop_table_header::uop_table_header(const std::uint8_t *ptr):uop_table_header(){
        load(ptr);
    }
    //=========================================================================================
    uop_table_header::uop_table_header(std::istream &input):uop_table_header(){
        load(input);
    }
    
    //=========================================================================================
    auto uop_table_header::load(const std::uint8_t *ptr) ->void {
        if (ptr == nullptr){
            throw std::runtime_error("Unable to load table header: nullptr");
        }
        std::copy(ptr,ptr+4,reinterpret_cast<std::uint8_t*>(&number_entries));
        std::copy(ptr+4,ptr+12,reinterpret_cast<std::uint8_t*>(&next_table));
        
    }
    //=========================================================================================
    auto uop_table_header::load(std::istream &input) ->void {
        if (!input.good()){
            throw std::runtime_error("Unable to load table header: stream not good state");
        }
        input.read(reinterpret_cast<char*>(&number_entries),4);
        input.read(reinterpret_cast<char*>(&next_table),8);
    }
    //=========================================================================================
    auto uop_table_header::save(std::ostream &output) const ->void {
        if (!output.good()){
            throw std::runtime_error("Unable to save table header: stream not good state");
        }
        output.write(reinterpret_cast<const char*>(&number_entries),4);
        output.write(reinterpret_cast<const char*>(&next_table),8);
    }
    
    
    
    //=========================================================================================
    // uop_info
    //=========================================================================================
    //=========================================================================================
    auto uop_info::initialize()->void{
        if (uop_ptr != nullptr){
            valid_uop = validUOP(uop_ptr);
            if (valid_uop){
                std::copy(uop_ptr+12,uop_ptr+20,reinterpret_cast<std::uint8_t*>(&table_start));
                std::copy(uop_ptr+20,uop_ptr+24,reinterpret_cast<std::uint8_t*>(&max_table_size));
                std::copy(uop_ptr+24,uop_ptr+28,reinterpret_cast<std::uint8_t*>(&entry_count));
                
            }
        }
        else if (input.is_open()) {
            valid_uop = validUOP(input);
            if (valid_uop){
                input.seekg(12,std::ios::beg);
                input.read(reinterpret_cast<char*>(&table_start),8);
                input.read(reinterpret_cast<char*>(&max_table_size),4);
                input.read(reinterpret_cast<char*>(&entry_count),4);
                
            }
        }
        else {
            throw std::runtime_error("UOP info has no valid data source");
        }
    }
    
    //=========================================================================================
    auto uop_info::loadTable() ->void {
        if (uop_ptr != nullptr){
            auto ptr = uop_ptr + table_start ;
            do {
                auto header = uop_table_header(ptr) ;
                ptr += uop_table_header::entry_length;
                for (auto j= 0 ; j < header.number_entries;j++){
                    auto entry = uop_table_entry(ptr) ;
                    if (entry.decompressed_length!=0){
                        table.push_back(entry);
                    }
                }
                ptr = uop_ptr + header.next_table ;
                
            }while(ptr != uop_ptr) ;
        }
        else if (input.good()){
            input.seekg(table_start,std::ios::beg);
            auto loop = true ;
            do {
                auto header = uop_table_header(input) ;
                
                for (auto j= 0 ; j < header.number_entries;j++){
                    auto entry = uop_table_entry(input) ;
                    if (entry.decompressed_length!=0){
                        table.push_back(entry);
                    }
                }
                loop = static_cast<bool>(header.next_table) ;
                if (loop){
                    input.seekg(header.next_table,std::ios::beg) ;
                }
                
            }while(loop) ;
        }
        else {
            throw std::runtime_error("uop_info load table fail: no valid input");
        }
    }
    //=========================================================================================
    uop_info::uop_info():uop_ptr(nullptr),valid_uop(false),max_table_size(0),entry_count(0),table_start(0){
    }
    //=========================================================================================
    uop_info::uop_info(const std::uint8_t *ptr ):uop_info(){
        uop_ptr = ptr ;
        initialize();
    }
    //=========================================================================================
    uop_info::uop_info(const std::filesystem::path &path):uop_info() {
        input = std::ifstream(path.string(),std::ios::binary);
        if (!input.is_open()){
            throw std::runtime_error("Unable to open: "s+path.string());
        }
        initialize();
    }
    //=========================================================================================
    auto uop_info::valid() const ->bool {
        return valid_uop;
    }
    //=========================================================================================
    auto uop_info::size() const ->std::uint32_t {
        return entry_count;
    }
    //=========================================================================================
    auto uop_info::maxTableSize() const ->std::uint32_t {
        return max_table_size ;
        
    }
    //=========================================================================================
    auto uop_info::firstTable() const ->std::uint64_t {
        return table_start;
    }
    
    //=========================================================================================
    auto uop_info::streamsrc() ->std::ifstream & {
        return input;
    }
    //=========================================================================================
    auto uop_info::memsrc() ->const std::uint8_t * {
        return uop_ptr;
    }
    //=========================================================================================
    auto uop_info::operator[](std::uint32_t index) const ->const uop_table_entry& {
        return table.at(index) ;
    }
    //=========================================================================================
    auto uop_info::operator[](std::uint32_t index) ->uop_table_entry& {
        return table.at(index) ;
    }

    //=========================================================================================
    // Generic uop methods
    //=========================================================================================
    
    //=========================================================================================
    auto validUOP(const std::uint8_t *ptr) ->bool {
        auto sig = std::uint32_t(0);
        auto ver = std::uint32_t(0);
        if (ptr == nullptr){
            return false ;
        }
        std::copy (ptr,ptr+4,reinterpret_cast<std::uint8_t*>(&sig));
        std::copy(ptr+4,ptr+8,reinterpret_cast<std::uint8_t*>(&ver));
        
        return (sig == uop_identifer) && (ver <= uop_version) ;
    }
    //=========================================================================================
    auto validUOP(std::istream &input) ->bool {
        auto sig = std::uint32_t(0);
        auto ver = std::uint32_t(0);
        input.read(reinterpret_cast<char*>(&sig),4);
        input.read(reinterpret_cast<char*>(&ver),4);
        if (!input.good() || (input.gcount()!=4)){
            return false ;
        }
        return (sig == uop_identifer) && (ver <= uop_version) ;
    }
    
    //===========================================================================================
    auto compressData(const std::vector<std::uint8_t> &data) ->std::vector<std::uint8_t> {
        auto sourcesize = static_cast<uLong>(data.size());
        auto roughsize = compressBound(sourcesize);
        auto dest = std::vector<std::uint8_t>(roughsize,0) ;
        auto status = compress(dest.data(), &roughsize, data.data(), sourcesize) ;
        if (status != Z_OK) {
            throw std::runtime_error("Error compressing data."s );
        }
        dest.resize(roughsize);
        return dest ;
    }
    //===========================================================================================
    auto compressData(const std::uint8_t *data,std::size_t size) ->std::vector<std::uint8_t> {
        auto sourcesize = static_cast<uLong>(size);
        auto roughsize = compressBound(sourcesize);
        auto dest = std::vector<std::uint8_t>(roughsize,0) ;
        auto status = compress(dest.data(), &roughsize, data, sourcesize) ;
        if (status != Z_OK) {
            throw std::runtime_error("Error compressing data."s );
        }
        dest.resize(roughsize);
        return dest ;
    }

    //===========================================================================================
    auto createUOP(std::ostream &output, std::uint32_t numentries,std::uint32_t maxtablesize,std::uint32_t version ) ->std::vector<std::uint64_t>{
        auto rvalue = std::vector<std::uint64_t>() ;
        if (output.good()){
            output.write(reinterpret_cast<const char*>(&uop_identifer),sizeof(uop_identifer));
            output.write(reinterpret_cast<const char*>(&version),sizeof(version));
            auto value = std::uint32_t(0xFD23EC43);// No idea if this needs to be this value, or 0, or whatever
            output.write(reinterpret_cast<char*>(&value),sizeof(value));
            // Were does the first table reside in the file?
            output.write(reinterpret_cast<const char*>(&starting_offset),sizeof(starting_offset));
            // table size
            output.write(reinterpret_cast<const char*>(&maxtablesize),sizeof(maxtablesize));
            // Number of entries
            output.write(reinterpret_cast<const char*>(&numentries),sizeof(numentries));
            // No idea if the next values need to be 1, 0 , or doesnt matter, so will copy what one was
            value = 1 ;
            output.write(reinterpret_cast<char*>(&value),sizeof(value));
            output.write(reinterpret_cast<char*>(&value),sizeof(value));
            value = 0 ;
            output.write(reinterpret_cast<char*>(&value),sizeof(value));
            
            // Ok, now we will fill in the rest of space until the table offset with zeros
            auto byte = std::uint8_t(0) ;
            auto temploc = static_cast<std::uint32_t>(output.tellp());
            for (std::uint32_t j= temploc ; j < starting_offset ; j++){
                output.write(reinterpret_cast<char*>(&byte),1);
            }
            // Ok, this is is the start of the tables
            // how many tables?
            auto numtables = numentries/maxtablesize + (numentries%maxtablesize>0?1:0) ;
            auto entry = uop_table_entry() ;
            auto entryoffset = std::uint64_t(0) ;
            for (std::uint32_t j=0 ; j< numtables;j++){
                // We need to write the table size
                auto tablesize = maxtablesize ;
                auto nextlocation = static_cast<std::uint64_t>(output.tellp()) + static_cast<std::uint64_t>(uop_table_entry::entry_size * tablesize) + 12 ;// 12 is the tablesize and nextlocation sizes
                if (numentries<maxtablesize){
                    tablesize = numentries ;
                    nextlocation = 0 ;
                }
                output.write(reinterpret_cast<const char*>(&tablesize),sizeof(tablesize));
                output.write(reinterpret_cast<const char*>(&nextlocation),sizeof(nextlocation));
                numentries -= tablesize ;
                for (std::uint32_t i=0 ; i < tablesize;i++){
                    entryoffset = static_cast<std::uint64_t>(output.tellp()) ;
                    rvalue.push_back(entryoffset);
                    entry.save(output);
                }
            }
        }
        return rvalue ;
    }
    
    //===========================================================================================
    auto readUOPData(const uop_table_entry &entry, std::istream &input) ->std::vector<std::uint8_t> {
        auto buffer = std::vector<std::uint8_t>(entry.compressed_length,0) ;
        input.seekg(entry.file_offset+entry.header_length,std::ios::beg);
        input.read(reinterpret_cast<char*>(buffer.data()),buffer.size());
        if (entry.use_compression){
            auto temp = std::vector<std::uint8_t>(entry.decompressed_length,0);
            uLong destlen = static_cast<uLongf>(temp.size()) ;
            uLong sourcelen = static_cast<uLongf>(buffer.size()) ;
            auto status = uncompress2(reinterpret_cast<Bytef *>(temp.data()), &destlen, reinterpret_cast<const Bytef *>(buffer.data()), &sourcelen);
            if (status != Z_OK){
                throw std::runtime_error("Error decompressing data.");
            }
            temp.resize(destlen);
            std::swap(buffer,temp);
        }
        return buffer ;
    }
    //===========================================================================================
    auto readUOPData(const uop_table_entry &entry, const std::uint8_t *ptr) ->std::vector<std::uint8_t> {
        auto buffer = std::vector<std::uint8_t>(entry.compressed_length,0) ;
        auto dataptr = ptr + entry.file_offset+entry.header_length;
        std::copy(dataptr,dataptr+buffer.size(),buffer.data());
        if (entry.use_compression){
            auto temp = std::vector<std::uint8_t>(entry.decompressed_length,0);
            uLong destlen = static_cast<uLongf>(temp.size()) ;
            uLong sourcelen = static_cast<uLongf>(buffer.size()) ;
            auto status = uncompress2(reinterpret_cast<Bytef *>(temp.data()), &destlen, reinterpret_cast<const Bytef *>(buffer.data()), &sourcelen);
            if (status != Z_OK){
                throw std::runtime_error("Error decompressing data.");
            }
            temp.resize(destlen);
            std::swap(buffer,temp);
        }
        return buffer ;
    }

}
