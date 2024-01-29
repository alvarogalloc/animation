export module game.input_manager;
import ext.sfml;
import ext.ginseng;

export namespace game::systems {
void handle_input(ginseng::database &, const sf::Event &);
}

module :private;
namespace game::systems {
void handle_input(ginseng::database &db, const sf::Event &ev)
{
  switch (ev.type)
  {
  case sf::Event::KeyPressed:
    db.visit([&](sf::View &view) {
      const float zoom_inc = 1.01f;
      const float zoom_dec = 0.99f;
      if (ev.key.code == sf::Keyboard::Period) view.zoom(zoom_inc);
      if (ev.key.code == sf::Keyboard::Comma) view.zoom(zoom_dec);
    });
    break;
  default:
    break;
  }
}
}// namespace game::system
