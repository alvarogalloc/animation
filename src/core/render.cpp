export module render;
import sfml;
import ginseng;
import components;
import game;
import tilemap;
import say;
import box2d;
import physics_debug_draw;

export namespace render {
class system
{
  sf::RenderWindow *win = nullptr;
  DebugDraw debug_draw;

public:
  system() = default;
  void startup(ginseng::database &db);
  void update(ginseng::database &db);
};
}// namespace render

module :private;

void render::system::startup(ginseng::database &db)
{
  db.visit([&](sf::RenderWindow *window, b2World *world) {
    win = window;
    debug_draw.setWindow(window);
    world->SetDebugDraw(&debug_draw);
    debug_draw.SetFlags(b2Draw::e_shapeBit);
  });
}

void render::system::update(ginseng::database &db)
{
  if (win == nullptr)
  {
    say::error("window is a nullptr, cant draw anything!!");
    return;
  }
  db.visit([&](components::game_tag, components::color &color) { win->clear(color); });

  db.visit([&](components::tilemap &tilemap) { win->draw(tilemap); });

  db.visit([&](components::sprite &sprite,
             ginseng::optional<components::render_box> render_box) {
    // TODO: check why i need to put origin to center
    sprite.setOrigin(sprite.getLocalBounds().width / 2.0f,
      sprite.getLocalBounds().height / 2.0f);
    win->draw(sprite);

    if (render_box)
    {
      sf::RectangleShape borderRect(sf::Vector2f(
        sprite.getLocalBounds().width, sprite.getLocalBounds().height));
      borderRect.setOrigin(sprite.getOrigin());
      borderRect.setScale(sprite.getScale());
      borderRect.setPosition(sprite.getPosition());
      borderRect.setFillColor(sf::Color::Transparent);
      borderRect.setOutlineColor(sf::Color::Red);
      borderRect.setOutlineThickness(2.0f);// You can change the thickness
      win->draw(borderRect);
    }
  });
  db.visit([&](b2World *world) { world->DebugDraw(); });
}
