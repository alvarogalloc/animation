module;
#include <cstdint>
#include <imgui.h>
#include <string>
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
import game.knight;
import core.systemapi;

export namespace game::entities {
void create_level(core::systemapi *api);
void render_level_debug_gui(core::systemapi *api);
}// namespace game::entities
export namespace game::systems {
// this accesses for now only knight entities, as are considered the only
// dynamic object
void check_tilemap_collisions(core::systemapi *api);
}// namespace game::systems


module :private;

namespace game::entities {
void create_level(core::systemapi *api)
{
  // level entity has 3 components
  // - the tilemap, renderable with vertex array
  // - static box for physics
  // - tilemap definition
  // TODO: check when these last two change, to trigger new tilemap creation

  auto entity_id = api->database().create_entity();

  sf::Texture *texture;
  api->database().visit([&](core::assetmanager &assets) {
    texture =
      assets.get<sf::Texture>("adventure/Terrain/Terrain (16x16).png").get();
  });
  using namespace core::components;


  // clang-format off
  tilemap_def definition
  {
    .rows = 20,
    .columns = 30,
    .tile_separation = 0,
    .tile_size = {16.f, 16.f},
    .tiles = {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      177,178,178,178,178,178,178,178,178,178,178,178,179,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      199,200,200,200,200,200,200,200,200,200,200,200,202,179,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      199,200,200,200,200,200,200,200,200,200,200,200,200,202,179,0,0,0,0,36,37,0,0,0,0,0,0,0,0,0,
      199,200,200,200,200,200,200,200,200,200,200,200,200,200,202,179,0,0,0,58,59,0,0,0,0,0,0,0,0,0,
      199,200,200,200,200,200,200,200,200,200,200,200,200,200,200,202,178,178,178,178,178,178,178,178,178,178,178,178,178,179,
      199,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,201,
      199,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,201,
      199,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,201
    }
  };
  // clang-format on
  tilemap tilemap{ std::move(definition), texture };
  tilemap.setScale(2.f, 2.f);
  tilemap.move(-100.f, 0.f);
  api->database().add_component(entity_id, std::move(tilemap));


  api->database().visit([&](b2World *world) {
    auto physics_box =
      core::physics::create_static_box(world, { 11.f, 12.5f }, { 22.5f, 1.f });
    api->database().add_component(entity_id, std::move(physics_box));
  });
}

void render_level_debug_gui(core::systemapi *) {}
}// namespace game::entities

namespace game::systems {

void check_tilemap_collisions(core::systemapi *api)
{
  api->database().visit(
    [&](ginseng::require<game::entities::knight_tag>, sf::Sprite &sprite) {
      auto [x, y] = sprite.getPosition();
      ImGui::Begin("tilemap collision");
      ImGui::Text(
        "%s", fmt::format("Knights position on screen {} {}", x, y).c_str());
      ImGui::End();
    });
}
}// namespace game::systems
