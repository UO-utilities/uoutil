//Copyright © 2023 Charles Kerr. All rights reserved.

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <array>
#include <vector>

#include "tile.hpp"

using namespace std::string_literals ;
namespace uo{
    constexpr auto entries_per_block = 32 ;
    constexpr auto terrain_block = (terrain_info::size * entries_per_block) + 4 ;
    constexpr auto art_block = (art_info::size * entries_per_block) + 4 ;
    constexpr auto art_offset = 512 * terrain_block ;
    constexpr auto info_string_size = 20 ;
    constexpr auto tiledata_size = std::size_t(3188736);
    //=========================================================================================
    // base_info
    //=========================================================================================
    //=========================================================================================
    auto  base_info::loadName(std::istream &input) ->void {
        auto buffer = std::array<char,info_string_size+1>() ;
        buffer[info_string_size] = 0 ;
        input.read(buffer.data(),info_string_size);
        name = buffer.data();
    }
    //=========================================================================================
    auto  base_info::loadName(const std::uint8_t *ptr) ->void {
        auto buffer = std::array<char,info_string_size+1>() ;
        buffer[info_string_size] = 0 ;
        std::copy(ptr,ptr+20,reinterpret_cast<std::uint8_t*>(buffer.data()));
        name = buffer.data();
    }
    //=========================================================================================
    auto  base_info::saveName(std::ostream &output) const ->void {
        auto size = std::min(static_cast<std::size_t>(20),name.size());
        output.write(name.c_str(),size);
        if ((20-size) > 0){
            auto buffer = std::vector<char>(20-size,0);
            output.write(buffer.data(),buffer.size());
        }
    }
    //=========================================================================================
    auto  base_info::saveName( std::uint8_t *ptr) const ->void {
        auto size = std::min(static_cast<std::size_t>(20),name.size());
        std::copy(reinterpret_cast<const std::uint8_t*>(name.c_str()),reinterpret_cast<const std::uint8_t*>(name.c_str())+size,ptr );
        if ((20-size) > 0){
            auto buffer = std::vector<std::uint8_t>(20-size,0);
            std::copy(buffer.begin(),buffer.end(),ptr+size);
        }
        
    }
    
    
    //=========================================================================================
    // terrain_info
    //=========================================================================================
    //=========================================================================================
    auto terrain_info::header(std::ostream &output,const std::string &sep,const std::string &flagsep) ->void{
        output<<"name"<<sep<<"texid"<<sep;
        flag_t::header(output,flagsep);
    }
    //=========================================================================================
    terrain_info::terrain_info():texture_id(0xFFFF) {
        tiletype = tiletype_t::terrain;
    }
    //=========================================================================================
    terrain_info::terrain_info(const std::uint8_t *ptr):terrain_info() {
        this->load(ptr);
    }
    //=========================================================================================
    terrain_info::terrain_info(std::istream &input) {
        this->load(input);
    }
    //=========================================================================================
    auto terrain_info::column(std::ostream &output,const std::string &sep, const std::string &flagsep) const ->void {
        output<<name<<sep<<texture_id<<sep;
        flag.column(output,flagsep);
    }
    //=========================================================================================
    auto terrain_info::description(std::ostream &output) const ->void {
        output <<"\tname  = "<<name<<"\n";
        output <<"\ttexid = "<<texture_id<<"\n";
        output <<"\tflag  = "<<flag.description()<<std::endl;
    }
    //=========================================================================================
    auto terrain_info::load(const std::uint8_t * ptr) ->void {
        std::copy(ptr,ptr+8,reinterpret_cast<std::uint8_t*>(&flag.value));
        std::copy(ptr+8,ptr+10,reinterpret_cast<std::uint8_t*>(&texture_id));
        loadName(ptr+10);
    }
    //=========================================================================================
    auto terrain_info::load(std::istream &input) ->void{
        input.read(reinterpret_cast<char*>(&flag.value),8) ;
        input.read(reinterpret_cast<char*>(&texture_id),2);
        loadName(input);
    }
    //=========================================================================================
    auto terrain_info::save( std::uint8_t * ptr) const ->void {
        std::copy(reinterpret_cast<const std::uint8_t*>(&flag.value),reinterpret_cast<const std::uint8_t*>(&flag.value)+8,ptr );
        std::copy(reinterpret_cast<const std::uint8_t*>(&texture_id),reinterpret_cast<const std::uint8_t*>(&texture_id)+2,ptr+8 );
        saveName(ptr+10) ;
    }
    //=========================================================================================
    auto terrain_info::save(std::ostream &output) const ->void {
        output.write(reinterpret_cast<const char*>(&flag.value),8);
        output.write(reinterpret_cast<const char*>(&texture_id),2);
        saveName(output) ;
    }
    //=========================================================================================
    // art_info
    //=========================================================================================
    //=========================================================================================
    auto art_info::header(std::ostream &output,const std::string &sep,const std::string &flagsep) ->void{
        output<<"name"<<sep<<"weight"<<sep<<"quality"<<sep<<"misc_data"<<sep<<"unknown2"<<sep<<"quantity"<<sep<<"animid"<<sep<<"unknown3"<<sep<<"hue"<<sep<<"stacking_offset"<<sep<<"height"<<sep;
        flag_t::header(output,flagsep);
    }
    //=========================================================================================
    art_info::art_info(): weight(0),quality(0),misc_data(0),unknown2(),quantity(0),animid(0),unknown3(0),hue(0),stacking_offset(),height(0){
        tiletype = tiletype_t::art;
    }
    //=========================================================================================
    art_info::art_info(const std::uint8_t * ptr) :art_info(){
        this->load(ptr);
    }
    //=========================================================================================
    art_info::art_info(std::istream &input) :art_info(){
        this->load(input);
    }

    //=========================================================================================
    auto art_info::column(std::ostream &output,const std::string &sep, const std::string &flagsep) const->void {
        output<<name<<sep<<static_cast<std::uint16_t>(weight)<<sep<<static_cast<std::uint16_t>(quality)<<sep<<misc_data<<sep<<static_cast<std::uint16_t>(unknown2)<<sep<<static_cast<std::uint16_t>(quantity)<<sep<<animid<<sep<<static_cast<std::uint16_t>(unknown3)<<sep<<static_cast<std::uint16_t>(hue)<<sep<<stacking_offset<<sep<<static_cast<std::uint16_t>(height)<<sep;
        flag.column(output,flagsep);
    }
    //=========================================================================================
    auto art_info::description(std::ostream &output) const ->void {
        output <<"\tname            = "<<name<<"\n";
        output <<"\tweight          = "<<static_cast<std::uint16_t>(weight)<<"\n";
        output <<"\tquality         = "<<static_cast<std::uint16_t>(quality)<<"\n";
        output <<"\tmisc_data       = "<<misc_data<<"\n";
        output <<"\tunknown2        = "<<static_cast<std::uint16_t>(unknown2)<<"\n";
        output <<"\tquantity        = "<<static_cast<std::uint16_t>(quantity)<<"\n";
        output <<"\tanimid          = "<<animid<<"\n";
        output <<"\tunknown3        = "<<static_cast<std::uint16_t>(unknown3)<<"\n";
        output <<"\thue             = "<<static_cast<std::uint16_t>(hue)<<"\n";
        output <<"\tstacking_offset = "<<stacking_offset<<"\n";
        output <<"\theight          = "<<static_cast<std::uint16_t>(height)<<"\n";
        output <<"\tflag            = "<<flag.description() << std::endl;

    }
    //=========================================================================================
    auto art_info::load(const std::uint8_t * ptr) ->void {
        std::copy(ptr,ptr+8,reinterpret_cast<std::uint8_t*>(&flag.value));
        std::copy(ptr+8,ptr+9,reinterpret_cast<std::uint8_t*>(&weight));
        std::copy(ptr+9,ptr+10,reinterpret_cast<std::uint8_t*>(&quality));
        std::copy(ptr+10,ptr+12,reinterpret_cast<std::uint8_t*>(&misc_data));
        std::copy(ptr+12,ptr+13,reinterpret_cast<std::uint8_t*>(&unknown2));
        std::copy(ptr+13,ptr+14,reinterpret_cast<std::uint8_t*>(&quantity));
        std::copy(ptr+14,ptr+16,reinterpret_cast<std::uint8_t*>(&animid));
        std::copy(ptr+16,ptr+17,reinterpret_cast<std::uint8_t*>(&unknown3));
        std::copy(ptr+17,ptr+18,reinterpret_cast<std::uint8_t*>(&hue));
        std::copy(ptr+18,ptr+20,reinterpret_cast<std::uint8_t*>(&stacking_offset));
        std::copy(ptr+20,ptr+21,reinterpret_cast<std::uint8_t*>(&height));
        loadName(ptr+21);
    }
    //=========================================================================================
    auto art_info::load(std::istream &input) ->void {
        input.read(reinterpret_cast<char*>(&flag.value),8) ;
        input.read(reinterpret_cast<char*>(&weight),sizeof(weight)) ;
        input.read(reinterpret_cast<char*>(&quality),sizeof(quality)) ;
        input.read(reinterpret_cast<char*>(&misc_data),sizeof(misc_data)) ;
        input.read(reinterpret_cast<char*>(&unknown2),sizeof(unknown2)) ;
        input.read(reinterpret_cast<char*>(&quantity),sizeof(quantity)) ;
        input.read(reinterpret_cast<char*>(&animid),sizeof(animid)) ;
        input.read(reinterpret_cast<char*>(&unknown3),sizeof(unknown3)) ;
        input.read(reinterpret_cast<char*>(&hue),sizeof(hue)) ;
        input.read(reinterpret_cast<char*>(&stacking_offset),sizeof(stacking_offset)) ;
        input.read(reinterpret_cast<char*>(&height),sizeof(height)) ;
        loadName(input);
    }
    //=========================================================================================
    auto art_info::save(std::uint8_t * ptr) const ->void {
        auto offset = 0 ;
        std::copy(reinterpret_cast<const std::uint8_t*>(&flag.value),reinterpret_cast<const std::uint8_t*>(&flag.value)+8,ptr );
        offset+= sizeof(flag.value);
        std::copy(reinterpret_cast<const std::uint8_t*>(&weight),reinterpret_cast<const std::uint8_t*>(&weight)+sizeof(weight),ptr+offset );
        offset+= sizeof(weight);
        std::copy(reinterpret_cast<const std::uint8_t*>(&quality),reinterpret_cast<const std::uint8_t*>(&quality)+sizeof(quality),ptr+offset );
        offset+= sizeof(quality);
        std::copy(reinterpret_cast<const std::uint8_t*>(&misc_data),reinterpret_cast<const std::uint8_t*>(&misc_data)+sizeof(misc_data),ptr+offset );
        offset+= sizeof(misc_data);
        std::copy(reinterpret_cast<const std::uint8_t*>(&unknown2),reinterpret_cast<const std::uint8_t*>(&unknown2)+sizeof(unknown2),ptr+offset );
        offset+= sizeof(unknown2);
        std::copy(reinterpret_cast<const std::uint8_t*>(&animid),reinterpret_cast<const std::uint8_t*>(&animid)+sizeof(animid),ptr+offset );
        offset+= sizeof(animid);
        std::copy(reinterpret_cast<const std::uint8_t*>(&unknown3),reinterpret_cast<const std::uint8_t*>(&unknown3)+sizeof(unknown3),ptr+offset );
        offset+= sizeof(unknown3);
        std::copy(reinterpret_cast<const std::uint8_t*>(&hue),reinterpret_cast<const std::uint8_t*>(&hue)+sizeof(hue),ptr+offset );
        offset+= sizeof(hue);
        std::copy(reinterpret_cast<const std::uint8_t*>(&stacking_offset),reinterpret_cast<const std::uint8_t*>(&stacking_offset)+sizeof(stacking_offset),ptr+offset );
        offset+= sizeof(stacking_offset);
        std::copy(reinterpret_cast<const std::uint8_t*>(&height),reinterpret_cast<const std::uint8_t*>(&height)+sizeof(height),ptr+offset );
        offset+= sizeof(height);
        saveName(ptr+offset);

    }
    //=========================================================================================
    auto art_info::save(std::ostream &output) const ->void{
        output.write(reinterpret_cast<const char*>(&flag.value),8);
        output.write(reinterpret_cast<const char*>(&weight),sizeof(weight));
        output.write(reinterpret_cast<const char*>(&quality),sizeof(quality));
        output.write(reinterpret_cast<const char*>(&misc_data),sizeof(misc_data));
        output.write(reinterpret_cast<const char*>(&unknown2),sizeof(unknown2));
        output.write(reinterpret_cast<const char*>(&animid),sizeof(animid));
        output.write(reinterpret_cast<const char*>(&unknown3),sizeof(unknown3));
        output.write(reinterpret_cast<const char*>(&hue),sizeof(hue));
        output.write(reinterpret_cast<const char*>(&stacking_offset),sizeof(stacking_offset));
        output.write(reinterpret_cast<const char*>(&height),sizeof(height));
        saveName(output) ;
    }

    //=========================================================================================
    // support routines
    //=========================================================================================
    //=========================================================================================
    auto offsetForTerrainTile(tileid_t tileid) ->std::uint64_t {
        auto offset = 0ull ;
        if (tileid == 0){
            return offset;
        }
        offset = ((tileid/entries_per_block) * terrain_block);
        offset += (((tileid % entries_per_block) * terrain_info::size) + 4) ;
        return offset ;
    }
    //=========================================================================================
    auto offsetForArtTile(tileid_t tileid) ->std::uint64_t {
        auto offset = art_offset ;
        offset += ((tileid/entries_per_block) * art_block) ;
        offset+= (((tileid%entries_per_block) * art_info::size)+4) ;
        return offset ;
    }
    
    //=========================================================================================
    auto initTileData(std::ostream &output) ->void {
        auto buffer = std::vector<char>(tiledata_size/1000,0) ;
        for (auto j=0;j<1000;j++){
            output.write(buffer.data(),buffer.size());
        }
        buffer =std::vector<char>(tiledata_size%1000,0);
        output.write(buffer.data(),buffer.size());
        output.seekp(0,std::ios::beg);
    }
    //=========================================================================================
    auto terrainTiles(std::istream &input) ->std::vector<terrain_info> {
        auto rvalue = std::vector<terrain_info>() ;
        input.seekg(0,std::ios::beg);
        for (tileid_t tileid = 0 ; tileid < terrain_tile_max;tileid++){
            if ((tileid == 1) || ((tileid!=0) && ((tileid%entries_per_block)==0))){
                input.seekg(4,std::ios::cur);
            }
            rvalue.push_back(terrain_info(input));
            if (!input.good()){
                throw std::runtime_error("Error reading terrain_info");
            }
        }
        return rvalue ;
    }
    //=========================================================================================
    auto artTiles(std::istream &input) ->std::vector<art_info> {
        auto rvalue = std::vector<art_info>();
        input.seekg(((entries_per_block*terrain_info::size)+4)*(terrain_tile_max/entries_per_block),std::ios::beg) ;
        for (tileid_t tileid = 0 ; tileid< invalid_tile; tileid++){
            if (tileid%entries_per_block == 0){
                input.seekg(4,std::ios::cur);
            }
            rvalue.push_back(art_info(input));
        }
        return rvalue ;
    }
    
    //=========================================================================================
    auto saveTerrain(std::ostream &output,const std::vector<terrain_info> &data )->void {
        output.seekp(0,std::ios::beg);
        tileid_t tileid=0 ;
        auto header = std::uint32_t(0);
        for (const auto &entry:data){
            if ((tileid==1) || ((tileid!=0) && ((tileid%entries_per_block)==0))){
                output.write(reinterpret_cast<char*>(&header),sizeof(header));
            }
            entry.save(output);
        }
        
    }
    //=========================================================================================
    auto saveArt(std::ostream &output,const std::vector<art_info> &data )->void {
        output.seekp(((entries_per_block*terrain_info::size)+4)*(terrain_tile_max/entries_per_block),std::ios::beg);
        tileid_t tileid=0 ;
        auto header = std::uint32_t(0);
        for (const auto &entry:data){
            if ((tileid%entries_per_block)==0){
                output.write(reinterpret_cast<char*>(&header),sizeof(header));
            }
            entry.save(output);
        }

    }

}
