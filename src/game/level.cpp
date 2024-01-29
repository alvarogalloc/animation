module;
#include <cstdint>
#include <imgui.h>
#include <utility>
#include <vector>
export module game.level;
import ext.sfml;
import ext.box2d;
import ext.ginseng;
import core.physics;
import core.assetmanager;
import core.say;
import core.tilemap;

export namespace game::entities {
void create_level(ginseng::database &db);
void render_level_debug_gui(ginseng::database &db);
}// namespace game::entities


module :private;

namespace game::entities {
void create_level(ginseng::database &db)
{
  // level entity has 3 components
  // - the tilemap, renderable with vertex array
  // - static box for physics
  // - tilemap definition
  // TODO: check when these last two change, to trigger new tilemap creation

  auto entity_id = db.create_entity();


  sf::Texture *texture;
  db.visit([&](core::assetmanager &assets) {
    texture = assets.get<sf::Texture>("tileset.png").get();
  });
  // add
  // clang-format off
  core::components::tilemap_def definition {
    .rows = 4,
    .columns = 4,
    .tile_separation = 1,
    .tile_size = sf::Vector2f {18.f, 18.f},
    .tiles = std::vector<std::size_t>{
      19,19, 19, 19,
      1, 1, 1, 1,
      0, 0, 0, 0,
      1, 2, 2, 3,
    }
  };
  // clang-format on
  core::components::tilemap tilemap{ definition, texture };
  tilemap.setScale(1.7778f, 1.7778f);
  tilemap.move(100.f, 100.f);
  db.add_component(entity_id, std::move(tilemap));

  db.add_component(entity_id, std::move(definition));

  db.visit([&](b2World *world) {
    auto physics_box =
      core::physics::create_static_box(world, { 12.5f, 12.5f }, { 22.5f, 1.f });
    db.add_component(entity_id, std::move(physics_box));
  });
}

void render_level_debug_gui(ginseng::database &db)
{
  auto edit_transform = [](auto &map) {
    std::array tmp{ map.getPosition().x, map.getPosition().y };
    ImGui::SliderFloat2("position", tmp.data(), 0, 100);
    map.setPosition(tmp[0], tmp[1]);
    tmp[0] = map.getScale().x;
    tmp[1] = map.getScale().y;
    ImGui::SliderFloat2("scale", tmp.data(), 0, 10);
    map.setScale(tmp[0], tmp[1]);
  };
  // returns whether it should create a new tileset
  // auto change_tile = [&](auto &definition, auto &map) {
  //   std::size_t i = 0;
  //   for (auto &tile : definition.tiles)
  //   {
  //     auto copy = static_cast<int>(tile);
  //     ImGui::SliderInt(fmt::format("tile number {}", i).c_str(), &copy, 1, 180);
  //     tile = static_cast<std::size_t>(copy);
  //     i++;
  //   }
  //   map.create_map(definition);
  // };

  db.visit([&](core::components::tilemap &map,
             [[maybe_unused]]core::components::tilemap_def &map_def) {
    ImGui::Begin("Tilemap");
    // make list of all the tiles
    // change_tile(map_def, map);
    edit_transform(map);
    ImGui::End();
  });
}
}// namespace game::entities
