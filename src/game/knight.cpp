module;
#include <string_view>
#include <unordered_map>
export module game.knight;
import ext.ginseng;
import ext.sfml;
import ext.box2d;
import core.components;
import core.say;
import core.assetmanager;
import core.animation;
import core.physics;

export namespace game::entities {
using knight = ginseng::tag<struct knight_tag>;
constexpr float knight_velocity = 150.f;
void spawn_knight(ginseng::database &db);
}// namespace game::entities

export namespace game::systems {
void knight_move(ginseng::database &db);
}// namespace game::systems


module :private;

namespace game::entities {

void spawn_knight(ginseng::database &db)
{
  auto id = db.create_entity();
  auto sprite = core::components::sprite();
  sprite.setScale(2, 2);
  db.visit([&](core::assetmanager &assets) {
    const sf::IntRect actual_size{ 40, 40, 28, 40 };
    auto text = assets.get<sf::Texture>("_Idle.png");
    auto text_running = assets.get<sf::Texture>("_Run.png");
    auto text_jump = assets.get<sf::Texture>("_Jump.png");
    auto text_fall = assets.get<sf::Texture>("_Fall.png");
    sprite.setTexture(*text);
    core::components::animation idle{ 10, text.get(), 0.5f, actual_size };
    core::components::animation running{
      10, text_running.get(), 0.5f, actual_size
    };
    core::components::animation jump{ 3, text_jump.get(), 0.3f, actual_size };
    core::components::animation fall{ 3, text_fall.get(), 0.3f, actual_size };
    core::components::animations animations{ "IDLE" };
    animations.sequences.emplace("IDLE", idle);
    animations.sequences.emplace("RUN", running);
    animations.sequences.emplace("JUMP", jump);
    animations.sequences.emplace("FALL", fall);
    animations.current_sequence = "IDLE";
    db.add_component(id, animations);
  });
  db.visit([&](b2World *world) {
    core::physics::components::dynamic_body body{
      core::physics::create_dynamic_box(
        world, { 0.f, 0.f }, { 0.875f * 2.f, 1.25f * 2.f }, 100.f, 0.9f)
    };
    body.max_velocity = 8.f;
    db.add_component(id, body);
  });
  db.add_component(id, knight{});
  db.add_component(id, sprite);
}
}// namespace game::entities

namespace game::systems {

void knight_move(ginseng::database &db)
{
  db.visit([&](entities::knight,
             core::components::animations &animations,
             core::physics::components::dynamic_body &body) {
    using enum core::physics::components::move_direction;
    switch (body.direction)
    {
    case LEFT:
      animations.current_sequence = "RUN";
      break;
    case RIGHT:
      animations.current_sequence = "RUN";
      break;
    case STOP:
      animations.current_sequence = "IDLE";
      break;
    default:
      break;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
      body.direction = LEFT;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
      body.direction = RIGHT;
    else
      body.direction = STOP;
  });
}
}// namespace game::systems
