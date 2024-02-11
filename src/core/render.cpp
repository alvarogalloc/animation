export module core.render;
import ext.sfml;
import ext.ginseng;
import ext.box2d;
import core.game;
import core.say;
import core.physics_debug_draw;
import core.tilemap;

export namespace core::render {
class system
{
  sf::RenderWindow *win = nullptr;
  DebugDraw debug_draw;

public:
  system() = default;
  void startup(ginseng::database &db);
  void update(ginseng::database &db);
};
}// namespace core::render

module :private;
namespace core::render {

void system::startup(ginseng::database &db)
{
  db.visit([&](ginseng::database::ent_id id, sf::RenderWindow *window, b2World *world) {
    win = window;
    debug_draw.setWindow(window);
    db.add_component(id, sf::View{window->getDefaultView()});
    world->SetDebugDraw(&debug_draw);
    debug_draw.SetFlags(b2Draw::e_shapeBit);
  });
}

void system::update(ginseng::database &db)
{
  if (win == nullptr)
  {
    say::error("window is a nullptr, cant draw anything!!");
    return;
  }
  db.visit(
    [&](components::game_tag, const sf::View& view, ginseng::optional<sf::Color> color) {
      win->setView(view);
      win->clear(color ? *color : sf::Color::Black);
    });
  db.visit([&](components::tilemap &map){
    // TODO: fixme for 
    win->draw(map);
  });


  db.visit([&](sf::Sprite &sprite) {
    // TODO: check why i need to put origin to center
    sprite.setOrigin(sprite.getLocalBounds().width / 2.0f,
      sprite.getLocalBounds().height / 2.0f);
    win->draw(sprite);
  });
  db.visit([&](b2World *world) { world->DebugDraw(); });
}
}// namespace core::render
