module;
#include <cmath>
#include <memory>
#include <utility>
export module core.physics;
import ext.ginseng;
import ext.sfml;
import core.game;
import core.say;
import ext.box2d;

export namespace core::physics {
// world vectors will be upsacled by this
constexpr float upfactor{ 32.0f };
constexpr float gravity{ 10.f };
constexpr float pi{ 3.1416f };

namespace components {
  enum class move_direction : std::uint8_t {
    STOP = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT,
  };
  struct static_body
  {
    b2Body *body;
  };
  struct dynamic_body
  {
    b2Body *body;
    float max_velocity{ 10.f };
    // first should be for the horizontal
    // second for vertical
    std::pair<move_direction, move_direction> directions{ move_direction::STOP,
      move_direction::STOP };
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


void update_sprite(const components::dynamic_body &body, sf::Sprite &sprite)
{
  sprite.setPosition(world_to_coords<float>(body.body->GetPosition()));
  sprite.setRotation(body.body->GetAngle() * 180 / pi);
}


void system::update(ginseng::database &db)
{
  world->Step(frame_time, velocity_iterations, position_iterations);
  db.visit([&](components::dynamic_body &body, sf::Sprite &sprite) {
    update_sprite(body, sprite);
  });
}
}// namespace core::physics
