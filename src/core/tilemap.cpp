module;
#include <cstdint>
export module tilemap;
import sfml;
import components;

export namespace components {
struct tilemap
  : public sf::Drawable
  , public sf::Transformable
{
  tilemap(sf::Texture *text,
    sf::Vector2f tileSize,
    const int *tiles,
    std::uint8_t width,
    std::uint8_t height,
    std::uint8_t tile_separation = 0);

  virtual void draw(sf::RenderTarget &target,
    sf::RenderStates states) const override;
  sf::Texture *tileset;
  sf::VertexArray vertices;
  
  sf::Vector2f tileSize;
  int *tiles;
  std::uint8_t width;
  std::uint8_t height;
  std::uint8_t tile_separation;
};

}// namespace components


module :private;

// grabbed from https://www.sfml-dev.org/tutorials/2.6/graphics-vertex-array.php
components::tilemap::tilemap(sf::Texture *text,
  sf::Vector2f tileSize,
  const int *tiles,
  std::uint8_t width,
  std::uint8_t height,
  std::uint8_t tile_separation)
{
  tileset = text;
  // resize the vertex array to fit the level size
  vertices.setPrimitiveType(sf::Triangles);
  // this means two triangles per tile
  vertices.resize(width * height * 6);

  for (std::uint8_t i = 0; i < width; ++i)
    for (std::uint8_t j = 0; j < height; ++j)
    {
      // get the current tile number
      int tileNumber = tiles[i + j * width];
      if (tileNumber == 0) continue;
      tileNumber--;

      // find its position in the tileset texture
      const int n_tiles_per_row =
        static_cast<int>(tileset->getSize().x) / static_cast<int>(tileSize.x);
      int tileset_column = tileNumber % n_tiles_per_row;
      int tileset_row = tileNumber / n_tiles_per_row;

      // get a pointer to the triangles' vertices of the current tile
      sf::Vertex *triangles = &vertices[(i + j * width) * 6];

      // define the 6 corners of the two triangles
      const sf::Vector2f first_position{ static_cast<float>(i * tileSize.x),
        static_cast<float>(j * tileSize.y) };

      triangles[0].position = first_position;
      triangles[1].position = first_position + sf::Vector2f{ tileSize.x, 0 };
      triangles[2].position = first_position + sf::Vector2f{ 0, tileSize.y };
      triangles[3].position = triangles[2].position;
      triangles[4].position = triangles[1].position;
      triangles[5].position =
        first_position + sf::Vector2f{ tileSize.x, tileSize.y };


      const sf::Vector2f total_separation = {
        static_cast<float>(tileset_column * tile_separation),
        static_cast<float>(tileset_row * tile_separation)
      };

      const auto start_position =
        sf::Vector2f{ static_cast<float>(tileset_column) * tileSize.x,
          static_cast<float>(tileset_row) * tileSize.y }
        + total_separation;

      // first triangle
      triangles[0].texCoords = start_position;
      triangles[1].texCoords = start_position + sf::Vector2f{ tileSize.x, 0 };
      triangles[2].texCoords = start_position + sf::Vector2f{ 0, tileSize.y };
      // second triangle
      triangles[3].texCoords = triangles[2].texCoords;
      triangles[4].texCoords = triangles[1].texCoords;
      triangles[5].texCoords =
        start_position + sf::Vector2f{ tileSize.x, tileSize.y };
    }
}

void components::tilemap::draw(sf::RenderTarget &target,
  sf::RenderStates states) const
{
  states.transform = getTransform();
  states.texture = tileset;
  target.draw(vertices, states);
}
