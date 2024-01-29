module;
#include <cmath>
#include <memory>
export module core.physics;
import ext.ginseng;
import ext.sfml;
import ext.box2d;
import core.components;

export namespace core::physics {
// world vectors will be upsacled by this
constexpr float upfactor{ 32.0f };
constexpr float gravity{ 5.f };
constexpr float pi{ 3.1416f };

namespace components {
  enum class move_direction : std::uint8_t {
    STOP,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT
  };
  struct static_body
  {
    b2Body *body;
  };
  struct dynamic_body
  {
    b2Body *body;
    float max_velocity{ 10.f };
    move_direction direction{ move_direction::STOP };
  };

}// namespace components

template<typename T> sf::Vector2<T> world_to_coords(b2Vec2 vec)
{
  return sf::Vector2<T>{ static_cast<T>(vec.x * upfactor),
    static_cast<T>(vec.y * upfactor) };
}
template<typename T> b2Vec2 coords_to_world(sf::Vector2<T> vec)
{
  return b2Vec2{ static_cast<float>(vec.x) / upfactor,
    static_cast<float>(vec.y) / upfactor };
}

std::unique_ptr<b2World> create_world()
{
  return std::make_unique<b2World>(b2Vec2(0.f, gravity));
}

components::static_body
  create_static_box(b2World *world, b2Vec2 position, b2Vec2 size);

components::dynamic_body create_dynamic_box(b2World *world,
  b2Vec2 position,
  b2Vec2 size,
  float density = 1.0f,
  float friction = 0.1f);

class system
{
public:
  system() : world(create_world()) {}
  std::unique_ptr<b2World> world;
  constexpr static float frame_time{ 1.f / 60.f };
  constexpr static int velocity_iterations{ 8 };
  constexpr static int position_iterations{ 3 };

  void startup(ginseng::database &db);

  void update(ginseng::database &db);
};

}// namespace core::physics


module :private;
namespace core::physics {
components::static_body
  create_static_box(b2World *world, b2Vec2 position, b2Vec2 size)
{
  b2BodyDef body_def{};
  body_def.position = position;
  b2Body *body = world->CreateBody(&body_def);
  b2PolygonShape box{};
  box.SetAsBox(size.x / 2.f, size.y / 2.f);
  body->CreateFixture(&box, 0.f);
  return components::static_body{ body };
}
components::dynamic_body create_dynamic_box(b2World *world,
  b2Vec2 position,
  b2Vec2 size,
  float density,
  float friction)
{

  b2BodyDef body_def{};
  body_def.position = position;
  body_def.type = b2_dynamicBody;
  b2Body *body = world->CreateBody(&body_def);
  b2PolygonShape box{};
  box.SetAsBox(size.x / 2.f, size.y / 2.f);
  b2FixtureDef fixture_def;
  fixture_def.shape = &box;
  fixture_def.density = density;
  fixture_def.friction = friction;
  body->CreateFixture(&fixture_def);
  return components::dynamic_body{ body };
}


void system::startup(ginseng::database &db)
{
  db.visit([&](ginseng::database::ent_id id, core::components::game_tag) {
    db.add_component(id, world.get());
  });
}

void system::update(ginseng::database &db)
{
  world->Step(frame_time, velocity_iterations, position_iterations);
  db.visit([&](components::dynamic_body &body, core::components::sprite &sprite) {
    const auto vel = body.body->GetLinearVelocity();
    float desired_vel{ 0.f };
    const float vel_increment = 0.5f;

    // accelerate to max velocity
    // through increments, that should be
    // different for each body but not for now haha
    using enum components::move_direction;
    switch (body.direction)
    {
    case LEFT:
      desired_vel = std::fmax(vel.x - vel_increment, -body.max_velocity);
      break;
    case STOP:
      if (std::abs(vel.x) <= 1.f)
        desired_vel = 0;
      else
        desired_vel =
          vel.x > 0.f ? vel.x - vel_increment : vel.x + vel_increment;

      break;
    case RIGHT:
      desired_vel = std::fmin(vel.x + vel_increment, body.max_velocity);
      break;
    default:
      break;
    }
    const float vel_change = desired_vel - vel.x;
    const float impulse = body.body->GetMass() * vel_change;
    body.body->ApplyLinearImpulse(
      b2Vec2(impulse, 0), body.body->GetWorldCenter(), true);

    // after step, set new data for drawing
    sprite.setPosition(world_to_coords<float>(body.body->GetPosition()));
    sprite.setRotation(body.body->GetAngle() * 180 / pi);
    // with the direction, if going left, flip the sprite, if going right,
    // reset it
    const auto scale = sprite.getScale();
    if (body.direction == LEFT)
    {
      sprite.setScale(-std::abs(scale.x), scale.y);
    } else if (body.direction == RIGHT)
    {
      sprite.setScale(std::abs(scale.x), scale.y);
    }
  });
}
}// namespace core::physics
