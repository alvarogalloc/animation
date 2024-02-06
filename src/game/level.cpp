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

  sf::Texture* texture;
  db.visit([&](core::assetmanager& assets){
     texture = assets.get<sf::Texture>("adventure/Terrain/Terrain (16x16).png").get();
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
  tilemap tilemap{std::move(definition), texture};
  tilemap.setScale(2.f, 2.f);
  tilemap.move(-100.f,0.f);
  db.add_component(entity_id, std::move(tilemap));


  db.visit([&](b2World *world) {
    auto physics_box =
      core::physics::create_static_box(world, { 12.5f, 12.5f }, { 22.5f, 1.f });
    db.add_component(entity_id, std::move(physics_box));
  });
}

void render_level_debug_gui(ginseng::database &) {}
}// namespace game::entities
