//Copyright © 2023 Charles Kerr. All rights reserved.

#ifndef image_hpp
#define image_hpp

#include <cstdint>
#include <string>
#include <vector>
#include <utility>
#include <filesystem>

#include "color.hpp"
//=========================================================================================
namespace uo {
    class image_t {
        std::vector<std::vector<color_t>> data ;
        
    public:
        /// Constructor for an image object.
        image_t() = default ;
        /// Constructor for an image object.
        /// Parameters:
        /// - height: the height of the image
        /// - width: the width of the image
        /// - fill: the color to fill the image with initially
        image_t(std::size_t height, std::size_t width,const color_t &fill = color_t()) ;
        /// Informs if the image is empty (has 0 height, or 0 width)
        /// - Returns: true if the image has no height or width
        auto empty() const ->bool ;
        /// Provides the size of the image
        /// - Returns: returns a pair that contains the height and width. NOTE that height is first
        auto size() const ->std::pair<std::size_t,std::size_t>;
        /// Allows one to resize an image (does not scale, just resizes)
        /// Parameters:
        /// - height: the height of the image
        /// - width: the width of the image
        /// - fill: the color to fill the image with initially
        /// - Returns: a reference to the image object
        [[maybe_unused]] auto size(std::size_t height, std::size_t width,const color_t &fill = color_t()) -> image_t& ;
        
        /// the color at a given row and col of the image
        /// Parameters:
        /// - row: the row for the color returned
        /// - col: the column for the color returned
        /// - Returns: A const reference to the color
        auto color(std::size_t row, std::size_t col) const -> const color_t& ;
        /// the color at a given row and col of the image
        /// Parameters:
        /// - row: the row for the color returned
        /// - col: the column for the color returned
        /// - Returns: A  reference to the color
        auto color(std::size_t row, std::size_t col)  ->  color_t& ;
        
        /// Flips the image around the veritical axis
        /// - Returns: a reference to the image object
        [[maybe_unused]] auto flip() ->image_t& ;
        ///  Inverts the image around the horizontal axis
        /// - Returns: a reference to the image object
        [[maybe_unused]] auto invert() ->image_t &;
        /// Generates a new image that is properly hued
        /// Parameters:
        /// - partial: specified if the hue requested is partial (grey only)
        /// - colors: a vector of color objects to use for hueing
        /// - Returns: a new hued image
        auto hue(bool partial , const std::vector<color_t> &colors) const -> image_t ;
        /// Generates a new image that is properly hued
        /// Parameters:
        /// - partial: specified if the hue requested is partial (grey only)
        /// - colors: a  pointer to 16 bit hue colors
        /// - Returns: a new hued image
        auto hue(bool partial , const std::uint8_t *colors) const -> image_t ;
    };
    /// Loads a bmp file and generates an image object
    /// The bmp file can not be compressed
    /// Parameters:
    /// - path: the path to the bmp file
    /// - Returns: a image based on the bmp file
    auto loadBMP(const std::filesystem::path &path) ->image_t ;
    /// Saves the image object to a bmp file
    /// The bmp file will be 16 bit color (native UO color)
    /// Parameters:
    /// - image: the image object to be saved
    /// - path: the path to the bmp file
    /// - Returns: true if able to create the file
    auto saveBMP(const image_t &image, const std::filesystem::path &path) ->bool ;
}
#endif /* image_hpp */
