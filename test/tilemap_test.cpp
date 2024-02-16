import ext.sfml;
import core.tilemap;
#define BOOST_UT_DISABLE_MODULE
#include <vector>
#include <boost/ut.hpp>


int main()
{
  using namespace boost::ut;
  using namespace boost::ut::spec;
  using namespace core;

  describe("tilemap") = [&] {
    sf::Texture text;
    // a 4*4 pixel texture with a 2*2 pixel tile
    // each tile is separated by 1 pixel
    text.create(5, 5);
    // fill the texture with a color
    sf::Image img;
    img.create(5, 5, sf::Color::Red);
    text.update(img);
    // create a tilemap
    // clang-format off
    std::vector<std::size_t> tiles{
      1, 1, 1, 1, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 1, 1, 1, 1
    };
    // clang-format on
    const sf::Vector2f tilesize{ 2, 2 };
    components::tilemap_def def{5,5,1,tilesize, std::move(tiles)};
    components::tilemap tilemap(def, &text);
      

    it("texture should not be null") = [&] {
      expect(tilemap.tileset != nullptr);
    };
    // it("texture should be 5*5") = [&] {
    //   expect(tilemap.tileset->getSize() == sf::Vector2u{ 5, 5 });
    // };
    // it("size of vertexarray should be size of tiles * 6") = [&] {
    //   expect(tilemap.vertices.getVertexCount() == 5 * 5 * 6);
    // };
    // it("last tile should be the right coordinates ") = [&] {
    //   // get six vertices of last tile
    //   std::array<sf::Vertex, 6> last_tile = {
    //     tilemap.vertices[5 * 5 * 6 - 6], tilemap.vertices[5 * 5 * 6 - 5],
    //     tilemap.vertices[5 * 5 * 6 - 4], tilemap.vertices[5 * 5 * 6 - 3],
    //     tilemap.vertices[5 * 5 * 6 - 2], tilemap.vertices[5 * 5 * 6 - 1]
    //   };
    //   expect(last_tile[0].position == sf::Vector2f{ 8, 8 });
    //   expect(last_tile[1].position == last_tile[4].position);
    //   expect(last_tile[2].position == last_tile[3].position);
    //   expect(last_tile[5].position == sf::Vector2f{ 10, 10 });
    // };
  };
  return 0;
}