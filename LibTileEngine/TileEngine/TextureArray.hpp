

#ifndef LIBTILEENGINE_TILEENGINE_TEXTUREARRAY_HPP
#define LIBTILEENGINE_TILEENGINE_TEXTUREARRAY_HPP

#include <memory>

#include <glm/vec2.hpp>

namespace TileEngine {

    /// A collection of textures stored in an OpenGL texture array.
    /// @note Assumes single channel textures.
    class TextureArray {
    public:
        /// Create an empty texture array with the given depth and resolution.
        /// @param depth The depth of the texture array, i.e., how many sub textures the array holds.
        /// @param resolution The width and height of each sub texture in pixels.
        /// @return An empty texture array.
        static std::unique_ptr<TextureArray> create(int depth, glm::ivec2 resolution);

        /// Create a texture array from an OpenGL ID.
        /// @param id The OpenGL ID for the texture array.
        explicit TextureArray(unsigned int id);

        TextureArray(TextureArray&) = delete; // Prevent copy to avoid issues with textures being freed via destructor.

        ~TextureArray();

        /// Load a texture into the texture array.
        /// @param zOffset The "depth" or "index" of the sub texture.
        /// @param bufferSize The width and height of the buffer in pixels.
        /// @param buffer The raw image buffer (single channel).
        void bufferSubImage(int zOffset, glm::ivec2 bufferSize, const unsigned char* buffer) const;

        /// Bind the texture array for rendering.
        void bind() const;

    private:
        /// The OpenGL ID for the texture array.
        const unsigned int m_id{};
    };

} // namespace TileEngine

#endif // LIBTILEENGINE_TILEENGINE_TEXTUREARRAY_HPP
