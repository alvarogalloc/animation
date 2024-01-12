#include <imgui-SFML.h>
#include <imgui.h>
import assetmanager;
import game;
import sfml;
import render;
import ginseng;
import components;
import animation;
import physics;
import knight;
import tilemap;
import box2d;
import say;


#ifdef _MSC_VER
#include <filesystem>
#endif
#include <array>

std::array level{ 1, 2, 3, 4 };

void make_map(ginseng::database &db)
{
  // clang-format off
  // std::array level{  
  //   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  //   0,0,0,0,0,0,127,0,0,0,0,0,0,0,0,0,0,0,0,0,
  //   0,0,0,0,2,3,3,3,4,0,0,0,0,0,0,0,0,0,0,0,
  //   0,0,0,0,0,0,0,0,0,0,15,14,14,13,14,14,16,0,0,0,
  //   0,0,0,0,0,0,0,0,0,0,0,0,0,33,0,0,0,0,0,0,
  //   0,0,0,0,0,0,0,0,0,0,0,0,0,53,0,0,0,0,0,0,
  //   0,0,0,0,0,0,0,0,0,0,0,2,3,3,3,3,3,3,4,0,
  //   0,0,0,0,0,0,0,0,0,0,0,0,0,53,0,0,0,0,0,0,
  //   0,0,0,0,0,125,0,0,0,0,0,0,0,53,0,0,0,0,0,0,
  //   0,0,0,0,2,3,3,3,4,0,0,0,0,53,0,0,0,0,0,0,
  //   0,0,0,0,0,0,0,0,0,0,0,0,0,53,0,0,0,0,0,0,
  //   0,0,0,0,0,0,0,0,0,0,0,0,0,73,0,0,0,0,0,0,
  //   0,0,0,0,0,0,0,0,0,0,0,2,3,3,3,4,0,0,0,0,
  //   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  //   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  //   0,0,0,0,0,125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  //   0,0,0,0,22,23,23,23,23,24,0,0,126,0,0,0,0,0,125,0,
  //   23,23,23,23,105,105,105,105,105,25,23,23,23,23,23,23,23,23,23,23,
  //   105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,
  //   105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105
  // };
  // clang-format on
  sf::Texture *text;
  db.visit([&](assetmanager &assets) {
    text = assets.get<sf::Texture>("tileset.png").get();
  });
  auto id = db.create_entity();
  const sf::Vector2f tile_size{ 18.f, 18.f };
  components::tilemap map{ text, tile_size, level.data(), 4, 1, 1 };
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
      db.visit([&](ginseng::database::ent_id id, components::game_tag) {
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
    .add_render_callback([&](ginseng::database &db) {
      db.visit([&](components::tilemap &map) {
        ImGui::Begin("Tilemap");
        std::array tmp{ map.getPosition().x, map.getPosition().y };
        ImGui::SliderFloat2("position", tmp.data(), 0, 100);
        map.setPosition(tmp[0], tmp[1]);
        tmp = { map.getScale().x, map.getScale().y };
        ImGui::SliderFloat2("scale", tmp.data(), 0, 10);
        map.setScale(tmp[0], tmp[1]);
        // make list of all the tiles
        int i = 0;
        for (auto &tile : level)
        {
          ImGui::SliderInt(
            fmt::format("tile number {}", i).c_str(), &tile, 1, 180);
          i++;
        }
        map = components::tilemap{
          map.tileset, { 18.f, 18.f }, level.data(), 4, 1, 1
        };
        map.setScale(tmp[0], tmp[1]);
        ImGui::End();
      });
    })
    .run();
  return 0;
}
