#include <exception>
#include <functional>
import core.assetmanager;
import core.game;
import core.render;
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
      .add_setup_callback(std::bind_front(&core::render::system::startup, &render_system))
      .add_setup_callback(game::entities::spawn_knight)
      .add_setup_callback(game::entities::create_level)
      .add_event_callback(game::systems::handle_input)
      .add_update_callback(game::systems::knight_move)
      .add_update_callback(
        std::bind_front(&core::physics::system::update, &physics_system))
      .add_update_callback(core::systems::update_animations)
      .add_render_callback(
        std::bind_front(&core::render::system::update, &render_system))
      .add_render_callback(game::systems::debug_controls)
      .add_render_callback(game::systems::knight_debug_info)
      .add_end_callback(
        std::bind_front(&core::physics::system::finish, &physics_system))
      .run();
  } catch (const std::exception &e)
  {
    core::say::error(fmt::format("Exception occurred {}", e.what()));
    game.teardown();
  }
  return 0;
}
