module;
#include <vector>
#include <cstdint>
export module game.terrain;
import core.assetmanager;
import ext.sfml;

export namespace game {
enum class Terrain : std::uint8_t {
  None = 0,
  Water,
  GrassLeftBorder,
  GrassRightBorder,
  Grass,
};


class MapLayer : public sf::Drawable {
  std::size_t rows;
  std::size_t cols;
  std::vector<Terrain> terrain;
  sf::Texture *texture;
  public:
  MapLayer(std::size_t _rows, std::size_t _cols, sf::Texture *_texture)
    : rows(_rows), cols(_cols), texture(_texture)
  {
  }
  constexpr sf::Sprite get_terrain_sprite(Terrain terrain)
  {
  // TODO, make this more flexible for different tilesets
  sf::Sprite sprite{ *texture };
  switch (terrain)
  {
  case Terrain::GrassLeftBorder:
    sprite.setTextureRect(sf::IntRect(96, 0, 16, 16));
    break;
  case Terrain::Grass:
    sprite.setTextureRect(sf::IntRect(112, 0, 16, 16));
    break;
  case Terrain::GrassRightBorder:
    sprite.setTextureRect(sf::IntRect(128, 0, 16, 16));
    break;
  case Terrain::None:
  default:
    sprite.setTextureRect(sf::IntRect(80, 0, 16, 16));
    break;
  }
  return sprite;
  }
};



constexpr std::vector<Terrain> generate_terrain(std::size_t rows, std::size_t cols,
  Terrain fill_with = Terrain::None)
{
  std::vector<Terrain> terrain(rows * cols, fill_with);
  return terrain;
}

sf::VertexArray generate_terrain_sprite(const MapLayer &terrain);
}// namespace game

module: private;

sf::VertexArray game::generate_terrain_sprite(const MapLayer &terrain)
{
  // resize the vertex array to fit the level size
  sf::VertexArray vertices{ sf::Triangles,  terrain.rows * terrain.cols * 6 };


}