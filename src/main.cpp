import game;
import components;
import animation;
import knight;
import physics;
import tilemap;
import render;
#include <box2d/box2d.h>
#ifdef _MSC_VER
#include <filesystem>

#endif

void make_map(ginseng::database &db)
{
  // clang-format off
  const int level[] ={  
  100,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,
  123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,
  123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,
  123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,
  123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,
  123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,
  123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,
  123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,
  123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,
  123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,
  123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,
  123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,
  123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,
  123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };
  // clang-format on
  sf::Texture *text;
  db.visit([&](assetmanager &assets) {
    text = assets.get<sf::Texture>("tileset.png").get();
  });
  auto id = db.create_entity();
  const auto tile_size = sf::Vector2i{ 18, 18 };
  components::tilemap map{ text, tile_size, level, 20, 20, 1 };
  // map.move(50, 50);
  map.setScale(2, 2);
  db.add_component(id, map);
  db.visit([&](b2World *world) {
    db.add_component(
      id, physics::create_static_box(world, { 12.5f, 12.5f }, { 22.5f, 1.f }));
  });
}

int main()
{
  physics::system physics_system;
  render::system render_system;
  game()
    .add_setup_callback(
      [&](ginseng::database &db) { physics_system.startup(db); })
    .add_setup_callback([&](ginseng::database &db) {
      render_system.startup(db);
      db.visit([&](ginseng::database::ent_id id, game::tag) {
        db.add_component(id, components::color{ sf::Color::Black });
      });
    })
    .add_setup_callback(entities::spawn_knight)
    .add_setup_callback(make_map)
    .add_update_callback(systems::knight_move)
    .add_update_callback(
      [&](ginseng::database &db) { physics_system.update(db); })
    .add_update_callback(systems::animations)
    .add_render_callback(
      [&](ginseng::database &db) { render_system.update(db); })
    .run();
  return 0;
}
