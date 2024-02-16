export module core.render;
import ext.sfml;
import ext.ginseng;
import ext.box2d;
import core.game;
import core.say;
import core.physics_debug_draw;
import core.tilemap;
import core.systemapi;

export namespace core::render {
class system
{
  sf::RenderWindow *win = nullptr;
  DebugDraw debug_draw;

public:
  system() = default;
  void startup(core::systemapi *api);
  void update(core::systemapi *api);
};
}// namespace core::render

module :private;
namespace core::render {

void system::startup(core::systemapi *api)
{
  api->database().visit([&](ginseng::database::ent_id id, sf::RenderWindow *window, b2World *world) {
    win = window;
    debug_draw.setWindow(window);
    api->database().add_component(id, sf::View{window->getDefaultView()});
    world->SetDebugDraw(&debug_draw);
    debug_draw.SetFlags(b2Draw::e_shapeBit);
  });
}

void system::update(core::systemapi *api)
{
  if (win == nullptr)
  {
    say::error("window is a nullptr, cant draw anything!!");
    return;
  }
  api->database().visit(
    [&](components::game_tag, const sf::View& view, ginseng::optional<sf::Color> color) {
      win->setView(view);
      win->clear(color ? *color : sf::Color::Black);
    });
  api->database().visit([&](components::tilemap &map){
    // TODO: fixme for 
    win->draw(map);
  });


  api->database().visit([&](sf::Sprite &sprite) {
    // TODO: check why i need to put origin to center
    sprite.setOrigin(sprite.getLocalBounds().width / 2.0f,
      sprite.getLocalBounds().height / 2.0f);
    win->draw(sprite);
  });
  api->database().visit([&](b2World *world) { world->DebugDraw(); });
}
}// namespace core::render
