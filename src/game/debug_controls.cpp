module;
#include <imgui.h>
export module game.debug_controls;
import core.game;
import ext.sfml;
import ext.ginseng;
import core.say;


export namespace game::systems {
void debug_controls(ginseng::database &db);
}

module :private;

namespace game::systems {

void debug_controls(ginseng::database &db)
{
  auto edit_vector2f = [](const char *label, sf::Vector2f v) -> sf::Vector2f {
    ImGui::Text("%s", label);
    ImGui::Indent();
    ImGui::InputFloat(fmt::format("x##{}", label).c_str(), &v.x);
    ImGui::InputFloat(fmt::format("y##{}", label).c_str(), &v.y);
    ImGui::Unindent();
    return v;
  };
  auto edit_rect4f = [](const char *label, sf::FloatRect r) -> sf::FloatRect {
    ImGui::Text("%s", label);
    ImGui::Indent();
    ImGui::InputFloat(fmt::format("left##{}", label).c_str(), &r.left);
    ImGui::InputFloat(fmt::format("top##{}", label).c_str(), &r.top);
    ImGui::InputFloat(fmt::format("width##{}", label).c_str(), &r.width);
    ImGui::InputFloat(fmt::format("height##{}", label).c_str(), &r.height);
    ImGui::Unindent();
    return r;
  };
  db.visit([&](sf::View &view) {
    // generate imgui debug for setting size of view
    ImGui::Begin("Debug Settings View");
    view.setSize(edit_vector2f("Size", view.getSize()));
    view.setCenter(edit_vector2f("Center", view.getCenter()));
    view.setViewport(edit_rect4f("Viewport", view.getViewport()));
    ImGui::End();
  });
}
}// namespace game::systems
