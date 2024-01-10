module;
#include <box2d/box2d.h>
export module box2d;
export  {
  using ::b2World;
  using ::b2AABB;
  using ::b2Transform;
  using ::b2Color;
  using ::b2Draw;
  using ::b2Body;
  using ::b2Vec2;
  using ::b2BodyDef;
  using ::b2PolygonShape;
  using ::b2FixtureDef;
  using ::operator-;
  using ::operator+;
  using ::operator*;
  using ::operator==;
  using ::operator!=;
}
