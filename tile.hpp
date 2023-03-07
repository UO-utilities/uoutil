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
    struct terrain_info :public base_info {
        constexpr static auto size = 30;
        std::uint16_t texture_id ;
        ~terrain_info()=default ;
        terrain_info() ;
        terrain_info(const std::uint8_t *ptr);
        terrain_info(std::istream &input) ;
        
        auto load(const std::uint8_t * ptr) ->void final ;
        auto load(std::istream &input) ->void final;
        auto save(std::uint8_t * ptr) const ->void final;
        auto save(std::ostream &output) const ->void final;
   };
    
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
        art_info() ;
        art_info(const std::uint8_t * ptr) ;
        art_info(std::istream &input) ;
        auto load(const std::uint8_t * ptr) ->void final ;
        auto load(std::istream &input) ->void final;
        auto save(std::uint8_t * ptr) const ->void final;
        auto save(std::ostream &output) const ->void final;

    };
    
    auto offsetForTerrainTile(tileid_t tileid) ->std::uint64_t ;
    auto offsetForArtTile(tileid_t tileid) ->std::uint64_t ;
    
    auto initTileData(std::ostream &output) ->void ;
 
    auto terrainTiles(std::istream &input) ->std::vector<terrain_info> ;
    auto artTiles(std::istream &input) ->std::vector<art_info> ;
    
    auto saveTerrain(std::ostream &output,const std::vector<terrain_info> &data )->void ;
    auto saveArt(std::ostream &output,const std::vector<art_info> &data )->void ;

}




#endif /* tile_hpp */
