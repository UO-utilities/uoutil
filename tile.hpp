//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef tile_hpp
#define tile_hpp

#include <cstdint>
#include <string>
#include <istream>
#include <ostream>

#include "general.hpp"
#include "flag.hpp"
//=========================================================================================
namespace uo{
    
    //=========================================================================================
    struct base_info {
    protected:
        auto loadName(std::istream &input) ->void ;
        auto loadName(const std::uint8_t *ptr) ->void ;
        auto saveName(std::ostream &output)const  ->void ;
        auto saveName(std::uint8_t *ptr) const ->void ;
    public:
        tiletype_t tiletype ;
        flag_t flag ;
        std::string name ;
        virtual ~base_info() = default ;
        
        auto type() const ->tiletype_t {return tiletype;}
        virtual auto load(const std::uint8_t * ptr) ->void=0 ;
        virtual auto load(std::istream &input) ->void=0 ;
        virtual auto save(std::uint8_t * ptr) const ->void=0 ;
        virtual auto save(std::ostream &output) const ->void =0;
        
    };
    
    //=========================================================================================
    struct terrain_info :public base_info {
        constexpr static auto size = 30;
        std::uint16_t texture_id ;
        
        /// Generates on header listing out the value names for a terrain (name,texture id)
        /// suitable for a title entry for a csv file
        /// Parameters:
        /// - output: output stream to the text file
        /// - sep: separator to use between names
        /// - flagsep: separator to use for flag values (useful to be different if one wants to parse separately)
        /// - Returns: nothing
        static auto header(std::ostream &output,const std::string &sep,const std::string &flagsep) ->void ;
        
        ~terrain_info()=default ;
        /// Constructor for terrain info
        terrain_info() ;
        /// Constructor for terrain info.  Will load the entry from the data
        /// specified by the pointer
        /// Parameters:
        /// - ptr: Pointer to the data for the terrain structure
        terrain_info(const std::uint8_t *ptr);
        /// Constructor for terrain info.  Will load the entry from the data
        /// specified by the stream
        /// Parameters:
        /// - input: Stream to the data for the terrain structure
        terrain_info(std::istream &input) ;
        
        /// Prints out the structure values in a row. Useful for generating csv files
        /// Parameters:
        /// - output: stream to the text file to output to
        /// - sep: Separator to use between values
        /// - flagsep: Separator to use for the flag values
        auto column(std::ostream &output,const std::string &sep, const std::string &flagsep) const->void ;
        /// Prints out the structure values in a key = value format , on entry per line
        /// Parameters:
        /// - output: stream to the text file to output to
        auto description(std::ostream &output) const ->void ;
        /// Loads the values of the structure from the data that is pointed to
        /// Parameters:
        /// - ptr: Pointer to the data for the terrain structure
        auto load(const std::uint8_t * ptr) ->void final ;
        /// Loads the values of the structure from the data that is in the file the stream is at
        /// Parameters:
        /// - input: Stream to the data
        auto load(std::istream &input) ->void final;
        /// Writes the data to the file the pointer points to
        /// Parameters:
        /// - ptr: Pointer  to the data to be written to
        auto save(std::uint8_t * ptr) const ->void final;
        /// Writes the data to the file the stream points to
        /// Parameters:
        /// - output: Stream  to the data to be written to
        auto save(std::ostream &output) const ->void final;
    };
    
    //=========================================================================================
    struct art_info : public base_info {
        constexpr static auto size = 41 ;
        std::uint8_t weight;
        std::uint8_t quality ;
        std::uint16_t misc_data ;
        std::uint8_t unknown2 ;
        std::uint8_t quantity ;
        std::uint16_t animid ;
        std::uint8_t unknown3;
        std::uint8_t hue ;
        std::uint16_t stacking_offset ;
        std::uint8_t height ;
        /// Generates on header listing out the value names for a terrain (name,weight, etc)
        /// suitable for a title entry for a csv file
        /// Parameters:
        /// - output: output stream to the text file
        /// - sep: separator to use between names
        /// - flagsep: separator to use for flag values (useful to be different if one wants to parse separately)
        /// - Returns: nothing
        static auto header(std::ostream &output,const std::string &sep,const std::string &flagsep) ->void ;
        ~art_info()=default ;
        /// Constructor for the art info
        art_info() ;
        /// Constructor for art info.  Will load the entry from the data
        /// specified by the pointer
        /// Parameters:
        /// - ptr: Pointer to the data for the terrain structure
        art_info(const std::uint8_t * ptr) ;
        /// Constructor for art info.  Will load the entry from the data
        /// specified by the stream
        /// Parameters:
        /// - input: Stream to the data for the art structure
        art_info(std::istream &input) ;
        /// Prints out the structure values in a row. Useful for generating csv files
        /// Parameters:
        /// - output: stream to the text file to output to
        /// - sep: Separator to use between values
        /// - flagsep: Separator to use for the flag values
        auto column(std::ostream &output,const std::string &sep,const std::string &flagsep) const ->void ;
        /// Prints out the structure values in a key = value format , on entry per line
        /// Parameters:
        /// - output: stream to the text file to output to
        auto description(std::ostream &output) const ->void ;
        /// Loads the values of the structure from the data that is pointed to
        /// Parameters:
        /// - ptr: Pointer to the data for the structure
        auto load(const std::uint8_t * ptr) ->void final ;
        /// Loads the values of the structure from the data that is in the file the stream is at
        /// Parameters:
        /// - input: Stream to the data
        auto load(std::istream &input) ->void final;
        /// Writes the data to the file the pointer points to
        /// Parameters:
        /// - ptr: Pointer  to the data to be written to
        auto save(std::uint8_t * ptr) const ->void final;
        /// Writes the data to the file the stream points to
        /// Parameters:
        /// - output: Stream  to the data to be written to
        auto save(std::ostream &output) const ->void final;
        
    };
    
    //=========================================================================================
    /// Generates an offset for the terrain tile id
    /// Can be used for offseting in a stream (or pter) to the correct entry
    /// Parameters:
    ///     - tileid: the id for the tile
    auto offsetForTerrainTile(tileid_t tileid) ->std::uint64_t ;
    /// Generates an offset for the art tile id
    /// Can be used for offseting in a stream (or pter) to the correct entry
    /// Parameters:
    ///     - tileid: the id for the tile. This is the art tile id (not offset by 0x4000)
    auto offsetForArtTile(tileid_t tileid) ->std::uint64_t ;
    
    /// For generating new tiledata.mul, this will create the proper size
    /// for the stream
     /// Parameters:
    ///     - output: The stream for the file to initailize the data
    auto initTileData(std::ostream &output) ->void ;
    
    /// Retrieves all terrain tile information
     /// Parameters:
    ///     - input: The stream for the file
    ///     - Returns: a vector of terrain information for each tile
    auto terrainTiles(std::istream &input) ->std::vector<terrain_info> ;
    /// Retrieves all art tile information
     /// Parameters:
    ///     - input: The stream for the file
    ///     - Returns: a vector of art information for each tile
    auto artTiles(std::istream &input) ->std::vector<art_info> ;
    
    /// Saves the vector of terrain tiles to the file
    /// Parameters:
    ///     - output: The stream for the file
    ///     - data: a vector of terrain information structures
    auto saveTerrain(std::ostream &output,const std::vector<terrain_info> &data )->void ;
    /// Saves the vector of art tiles to the file. Assumes the file has been initialized
    /// Parameters:
    ///     - output: The stream for the file
    ///     - data: a vector of art information structures
    auto saveArt(std::ostream &output,const std::vector<art_info> &data )->void ;
    
}




#endif /* tile_hpp */
