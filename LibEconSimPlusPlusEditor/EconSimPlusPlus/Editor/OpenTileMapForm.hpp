//  EconSimPlusPlus
//  A program that simulates the historical development of capitalist economies.
//  Copyright (C) 2024.   Anthony Dickson anthony.dickson9656@gmail.com
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by Anthony Dickson on 15/07/2024.
//

#ifndef OPENTILEMAPFORM_HPP
#define OPENTILEMAPFORM_HPP

#include <EconSimPlusPlus/Image.hpp>
#include <EconSimPlusPlus/Object.hpp>
#include <EconSimPlusPlus/Texture.hpp>


namespace EconSimPlusPlus::Editor {

    /// @brief A form to get the tile size from the user to properly configure a tilemap via the GUI.
    /// @details Tile map textures (image files) do not encode the size (width and height) of the individual tiles.
    /// While we could set a default tile size and allow the user to interactively set the tile size as needed, this may
    /// cause issues where tile IDs are invalidated and refer to now out-of-bounds indices, leading to crashes or the
    /// need for extensives checks. The other issue is that setting the tile size is an operation that only needs to be
    /// done once, and there is no reason to change the tile size during editing since there is only one correct
    /// setting.
    /// @note This form is intended to be displayed after the user has chosen a tile map texture via the GUI.
    class OpenTileMapForm final : public Object {
    public:
        static std::unique_ptr<OpenTileMapForm> create(const Image::Image& image, glm::vec2 initialTileSize, const Font* font,
                                                       const std::function<void(glm::vec2 tileSize)>& onConfirm,
                                                       const std::function<void()>& onCancel);

        /// Create a new form for configuring a tile map.
        /// @param texture The tile sheet texture.
        /// @param initialTileSize The tile size (width and height, in pixels) to start with.
        /// @param onConfirm The function to call if the user clicks the confirm button. This function will be passed
        /// the tile size that the user has entered.
        /// @param onCancel The function to call if the user clicks the cancel button.
        OpenTileMapForm(std::unique_ptr<Texture> texture, glm::vec2 initialTileSize,
                        const std::function<void(glm::vec2 tileSize)>& onConfirm,
                        const std::function<void()>& onCancel);

        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;
        void render(const Graphics& graphics) const override;

    private:
        /// The tile sheet texture.
        const std::unique_ptr<Texture> m_texture;
        /// The width and height of a tile in the til map.
        glm::vec2 m_tileSize;
        /// The function to call if the user clicks the confirm button.
        const std::function<void(glm::vec2 tileSize)> m_onConfirm;
        /// The function to call if the user clicks the cancel button.
        const std::function<void()> m_onCancel;
    };

} // namespace EconSimPlusPlus::Editor

#endif // OPENTILEMAPFORM_HPP
