#include <exception>
#include <functional>
import core.assetmanager;
import core.game;
import core.render;
import core.components;
import core.animation;
import core.physics;
import core.say;
import ext.ginseng;
import ext.box2d;
import game.knight;
import game.level;
import game.input_manager;
import game.debug_controls;


int main()
{
  core::physics::system physics_system;
  core::render::system render_system;

  core::game game;
  try
  {
    game
      .add_setup_callback(
        std::bind_front(&core::physics::system::startup, &physics_system))
      .add_setup_callback([&](ginseng::database &db) {
        render_system.startup(db);
        // db.visit([&](ginseng::database::ent_id id,
        // core::components::game_tag) {
        //   // db.add_component(id, core::components::color{ sf::Color::White
        //   });
        // });
      })
      .add_setup_callback(game::entities::spawn_knight)
      .add_setup_callback(game::entities::create_level)
      .add_update_callback(game::systems::knight_move)
      .add_event_callback(game::systems::handle_input)
      .add_update_callback(
        std::bind_front(&core::physics::system::update, &physics_system))
      .add_update_callback(core::systems::update_animations)
      .add_update_callback(
        std::bind_front(&core::render::system::update, &render_system))
      // .add_render_callback(game::entities::render_level_debug_gui)
      .add_render_callback(game::systems::debug_controls)
      .run();
  } catch (const std::exception &e)
  {
    core::say::error(fmt::format("Exception occurred {}", e.what()));
    game.teardown();
  }
  return 0;
}
