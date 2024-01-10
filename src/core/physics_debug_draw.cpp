module;
#include <cmath>
#include <cstdint>
export module physics_debug_draw;
import box2d;
import sfml;
export class DebugDraw : public b2Draw
{
  sf::RenderWindow *m_window{ nullptr };
  static constexpr float SCALE = 32.f;

  sf::Vector2f B2VecToSFVec(const b2Vec2 &vec)
  {
    return sf::Vector2f{ vec.x * 32.f, vec.y * 32.f };
  }
  sf::Color toSFColor(const b2Color &color)
  {
    return sf::Color{ static_cast<std::uint8_t>(color.r * 255),
      static_cast<std::uint8_t>(color.g * 255),
      static_cast<std::uint8_t>(color.b * 255),
      60 };
  }

public:
  DebugDraw() {}
  void setWindow(sf::RenderWindow *win) { m_window = win; }

  void
    DrawPolygon(const b2Vec2 *vertices, int vertexCount, const b2Color &color);

  void DrawSolidPolygon(const b2Vec2 *vertices,
    int vertexCount,
    const b2Color &color);

  void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color);

  void DrawSolidCircle(const b2Vec2 &center,
    float radius,
    const b2Vec2 &axis,
    const b2Color &color);

  void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color);

  void DrawTransform(const b2Transform &xf);

  void DrawPoint(const b2Vec2 &p, float size, const b2Color &color);

  void DrawString(int x, int y, const char *string, ...);

  void DrawAABB(b2AABB *aabb, const b2Color &color);
};

module :private;

void DebugDraw::DrawPolygon(const b2Vec2 *vertices,
  int vertexCount,
  const b2Color &color)
{
  sf::ConvexShape polygon(static_cast<std::size_t>(vertexCount));
  sf::Vector2f center;
  for (int i = 0; i < vertexCount; i++)
  {
    // polygon.setPoint(i, SFMLDraw::B2VecToSFVec(vertices[i]));
    sf::Vector2f transformedVec = DebugDraw::B2VecToSFVec(vertices[i]);
    polygon.setPoint(size_t(i),
      sf::Vector2f(std::floor(transformedVec.x),
        std::floor(transformedVec.y)));// flooring the coords to fix distorted
                                       // lines on flat surfaces
  }// they still show up though.. but less frequently
  polygon.setOutlineThickness(-1.f);
  polygon.setFillColor(sf::Color::Transparent);
  polygon.setOutlineColor(toSFColor(color));

  m_window->draw(polygon);
}
void DebugDraw::DrawSolidPolygon(const b2Vec2 *vertices,
  int vertexCount,
  const b2Color &color)
{
  sf::ConvexShape polygon(static_cast<std::size_t>(vertexCount));
  for (int i = 0; i < vertexCount; i++)
  {
    // polygon.setPoint(i, SFMLDraw::B2VecToSFVec(vertices[i]));
    sf::Vector2f transformedVec = DebugDraw::B2VecToSFVec(vertices[i]);
     polygon.setPoint(static_cast<std::size_t>(i),
      sf::Vector2f(std::floor(transformedVec.x),
        std::floor(transformedVec.y)));// flooring the coords to fix distorted
                                       // lines on flat surfaces
  }// they still show up though.. but less frequently
  polygon.setOutlineThickness(-1.f);
  polygon.setFillColor(toSFColor(color));
  polygon.setOutlineColor(toSFColor(color));

  m_window->draw(polygon);
}
void DebugDraw::DrawCircle(const b2Vec2 &center,
  float radius,
  const b2Color &color)
{
  sf::CircleShape circle(radius * DebugDraw::SCALE);
  circle.setOrigin(radius * DebugDraw::SCALE, radius * DebugDraw::SCALE);
  circle.setPosition(DebugDraw::B2VecToSFVec(center));
  circle.setFillColor(sf::Color::Transparent);
  circle.setOutlineThickness(-1.f);
  circle.setOutlineColor(toSFColor(color));

  m_window->draw(circle);
}
void DebugDraw::DrawSolidCircle(const b2Vec2 &center,
  float radius,
  const b2Vec2 &axis,
  const b2Color &color)
{
  sf::CircleShape circle(radius * DebugDraw::SCALE);
  circle.setOrigin(radius * DebugDraw::SCALE, radius * DebugDraw::SCALE);
  circle.setPosition(DebugDraw::B2VecToSFVec(center));
  circle.setFillColor(toSFColor(color));
  circle.setOutlineThickness(1.f);
  circle.setOutlineColor(toSFColor(color));

  b2Vec2 endPoint = center + radius * axis;
  sf::Vertex line[2] = {
    sf::Vertex(DebugDraw::B2VecToSFVec(center), toSFColor(color)),
    sf::Vertex(DebugDraw::B2VecToSFVec(endPoint), toSFColor(color)),
  };

  m_window->draw(circle);
  m_window->draw(line, 2, sf::Lines);
}
void DebugDraw::DrawSegment(const b2Vec2 &p1,
  const b2Vec2 &p2,
  const b2Color &color)
{
  sf::Vertex line[] = { sf::Vertex(
                          DebugDraw::B2VecToSFVec(p1), toSFColor(color)),
    sf::Vertex(DebugDraw::B2VecToSFVec(p2), toSFColor(color)) };

  m_window->draw(line, 2, sf::Lines);
}
void DebugDraw::DrawTransform(const b2Transform &xf)
{
  float lineLength = 0.4f;

  /*b2Vec2 xAxis(b2Vec2(xf.p.x + (lineLength * xf.q.c), xf.p.y + (lineLength *
   * xf.q.s)));*/
  b2Vec2 xAxis = xf.p + lineLength * xf.q.GetXAxis();
  sf::Vertex redLine[] = { sf::Vertex(
                             DebugDraw::B2VecToSFVec(xf.p), sf::Color::Red),
    sf::Vertex(DebugDraw::B2VecToSFVec(xAxis), sf::Color::Red) };

  // You might notice that the ordinate(Y axis) points downward unlike the one
  // in Box2D testbed That's because the ordinate in SFML coordinate system
  // points downward while the OpenGL(testbed) points upward
  /*b2Vec2 yAxis(b2Vec2(xf.p.x + (lineLength * -xf.q.s), xf.p.y + (lineLength *
   * xf.q.c)));*/
  b2Vec2 yAxis = xf.p + lineLength * xf.q.GetYAxis();
  sf::Vertex greenLine[] = { sf::Vertex(
                               DebugDraw::B2VecToSFVec(xf.p), sf::Color::Green),
    sf::Vertex(DebugDraw::B2VecToSFVec(yAxis), sf::Color::Green) };

  m_window->draw(redLine, 2, sf::Lines);
  m_window->draw(greenLine, 2, sf::Lines);
}

void DebugDraw::DrawPoint(const b2Vec2 &p, float size, const b2Color &color)
{
  // Convert Box2D coordinates to SFML coordinates (pixels)
  sf::Vector2f sfmlPos(p.x * SCALE, p.y * SCALE);

  // Create a circle shape to represent the point
  sf::CircleShape point(size * SCALE);
  point.setFillColor(toSFColor(color));
  point.setPosition(sfmlPos);

  // Draw the point onto the SFML window
  m_window->draw(point);
}
