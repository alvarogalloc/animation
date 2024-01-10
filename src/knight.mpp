module;
#include <string_view>
#include <unordered_map>
export module knight;
import ginseng;
import sfml;
import components;
import assetmanager;
import animation;
import physics;
import box2d;

export namespace entities {
using knight = ginseng::tag<struct knight_tag>;
constexpr float knight_velocity = 150.f;
void spawn_knight(ginseng::database &db);
}// namespace entities

export namespace systems {
void knight_move(ginseng::database &db);
}// namespace systems


module :private;

namespace entities {

void spawn_knight(ginseng::database &db)
{
  auto id = db.create_entity();
  auto sprite = components::sprite();
  sprite.setScale(2, 2);
  db.visit([&](assetmanager &assets) {
    const sf::IntRect actual_size{ 40, 40, 28, 40 };
    auto text = assets.get<sf::Texture>("_Idle.png");
    auto text_running = assets.get<sf::Texture>("_Run.png");
    auto text_jump = assets.get<sf::Texture>("_Jump.png");
    auto text_fall = assets.get<sf::Texture>("_Fall.png");
    sprite.setTexture(*text);
    components::animation idle{ 10, text.get(), 0.5f, actual_size };
    components::animation running{ 10, text_running.get(), 0.5f, actual_size };
    components::animation jump{ 3, text_jump.get(), 0.3f, actual_size };
    components::animation fall{ 3, text_fall.get(), 0.3f, actual_size };
    components::animations animations{ "IDLE" };
    animations.sequences.emplace("IDLE", idle);
    animations.sequences.emplace("RUN", running);
    animations.sequences.emplace("JUMP", jump);
    animations.sequences.emplace("FALL", fall);
    animations.current_sequence = "IDLE";
    db.add_component(id, animations);
  });
  db.visit([&](b2World *world) {
    physics::components::dynamic_body body{ physics::create_dynamic_box(
      world, { 0.f, 0.f }, { 0.875f * 2.f, 1.25f * 2.f }, 100.f, 0.9f) };
    body.max_velocity = 8.f;
    db.add_component(id, body);
  });
  db.add_component(id, knight{});
  db.add_component(id, sprite);
}
}// namespace entities

namespace systems {

void knight_move(ginseng::database &db)
{
  db.visit([&](entities::knight,
             components::animations &animations,
             physics::components::dynamic_body &body) {
    using enum physics::components::move_direction;
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
}// namespace systems
