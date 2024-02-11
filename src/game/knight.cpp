module;
#include <imgui.h>
#include <string>
#include <unordered_map>
export module game.knight;
import ext.ginseng;
import ext.sfml;
import ext.box2d;
import core.say;
import core.assetmanager;
import core.animation;
import core.physics;

export namespace game::entities {
using knight_tag = ginseng::tag<struct knight_tag_t>;
constexpr float knight_velocity = 150.f;
void spawn_knight(ginseng::database &db);
}// namespace game::entities

export namespace game::systems {
void knight_move(ginseng::database &db);
void knight_debug_info(ginseng::database &db);
}// namespace game::systems


module :private;

namespace game::entities {

void spawn_knight(ginseng::database &db)
{
  auto id = db.create_entity();
  auto sprite = sf::Sprite();
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
        world, { 0.f, 0.f }, { 0.875f * 2.f, 1.25f * 2.f }, 1.f, 0.9f)
    };
    body.max_velocity = 6.f;
    db.add_component(id, body);
  });
  db.add_component(id, knight_tag{});
  db.add_component(id, sprite);
}
}// namespace game::entities

namespace game::systems {

void knight_jump(core::physics::components::dynamic_body &body)
{
  const auto vy = body.body->GetLinearVelocity().y;
  // only if not moving vertically
  if (std::round(vy) == 0.f)
  {
    auto impulse = body.body->GetMass() * 8.f;
    body.body->ApplyLinearImpulse(
      b2Vec2(0, -impulse), body.body->GetWorldCenter(), true);
  }
}


void move_knight(core::physics::components::dynamic_body &body,
  core::physics::components::move_direction direction)
{
  const float vel_increment = 0.3f;
  float desired_vel = 0.f;
  const auto vx = body.body->GetLinearVelocity().x;
  switch (direction)
  {
    using enum core::physics::components::move_direction;
  case RIGHT:
    desired_vel = std::min(body.max_velocity, vx + vel_increment);
    break;
  case LEFT:
    desired_vel = std::max(-body.max_velocity, vx - vel_increment);
    break;
  case STOP:
    if (std::abs(vx) <= 1.f)
    {
      desired_vel = 0;
    } else
    {
      desired_vel = vx > 0.f ? vx - vel_increment : vx + vel_increment;
    }
    break;
  default:
    return;
  }
  float impulse = (desired_vel - vx) * body.body->GetMass();
  body.body->ApplyLinearImpulse(
    b2Vec2(impulse, 0), body.body->GetWorldCenter(), true);
}

void handle_knight_animation(sf::Sprite &sprite,
  core::components::animations &animations,
  const b2Vec2 &body_velocity)
{
  // change the animation in with these rules:
  // + vertical animations (falling, jumping) have priority over horizontal
  const auto [vx, vy] = body_velocity;
  constexpr float friction_tolerance = 0.01f;
  if ((vx > friction_tolerance or vx < -friction_tolerance)
      and (vy > -friction_tolerance and vy < friction_tolerance))
  {
    animations.current_sequence = "RUN";
    // handle sprite flipping in horizontal
    const auto scale = sprite.getScale();
    if (vx < 0.f)
    {
      sprite.setScale(-std::abs(scale.x), scale.y);
    } else if (vx > 0.f)
    {
      sprite.setScale(std::abs(scale.x), scale.y);
    }

  } else if (vy > friction_tolerance)
  {
    animations.current_sequence = "FALL";
  } else if (vy < -friction_tolerance)
  {
    animations.current_sequence = "JUMP";
  } else
  {
    animations.current_sequence = "IDLE";
  }
}

void knight_move(ginseng::database &db)
{
  db.visit([&](ginseng::require<entities::knight_tag>,
             sf::Sprite &sprite,
             core::components::animations &animations,
             core::physics::components::dynamic_body &body) {
    using enum core::physics::components::move_direction;
    auto move_dir = STOP;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
      move_dir = LEFT;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
      move_dir = RIGHT;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
      knight_jump(body);
    }

    move_knight(body, move_dir);
    handle_knight_animation(sprite, animations, body.body->GetLinearVelocity());
  });
}

void knight_debug_info(ginseng::database &db)
{
  db.visit([&](ginseng::require<game::entities::knight_tag>,
             sf::Sprite sprite,
             core::physics::components::dynamic_body body) {
    if (ImGui::BeginTabItem("knight debug info"))
    {
      auto [vx, vy] = body.body->GetLinearVelocity();
      ImGui::Text(
        "%s", fmt::format("velocity: x:{:.2f} y:{:.2f}", vx, vy).c_str());
      auto [x, y] = sprite.getPosition();
      ImGui::Text(
        "%s", fmt::format("position: x: {:.2f} y: {:.2f}", x, y).c_str());
      ImGui::EndTabItem();
    }
  });
}

}// namespace game::systems
