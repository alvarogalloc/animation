module;
#include <functional>
#include <stdexcept>
#include <imgui.h>
#include <filesystem>
export module core.game;
import core.assetmanager;
import core.utils;
import core.say;
import ext.ginseng;
import ext.sfml;
import core.systemapi;

export namespace core {
constexpr std::uint32_t win_width{ 640 };
constexpr std::uint32_t win_height{ 640 };
namespace components {
  using game_tag = ginseng::tag<struct game_tag_t>;
}

class game : public core::systemapi
{

public:
  using func_type = std::function<void(core::systemapi *)>;
  using event_func_type =
    std::function<void(core::systemapi *, const sf::Event &)>;
  enum class flow { running, stop };

  game();

  game &add_setup_callback(func_type func);
  game &add_end_callback(func_type func);
  game &add_update_callback(func_type func);
  game &add_render_callback(func_type func);
  game &add_event_callback(event_func_type func);

  void run();
  void teardown();

  // override systemapi
  sf::RenderWindow &window() override { return m_window; }
  sf::View &camera() override { return m_camera; }
  assetmanager &assets() override { return m_assets; }
  ginseng::database &database() override { return m_db; }

  // const accessors
  const sf::RenderWindow &window() const override { return m_window; }
  const sf::View &camera() const override { return m_camera; }
  const assetmanager &assets() const override { return m_assets; }
  const ginseng::database &database() const override { return m_db; }

private:
  utils::hook<func_type, core::systemapi *> m_hook_setup;
  utils::hook<event_func_type, core::systemapi *, const sf::Event &>
    m_hook_events;
  utils::hook<func_type, core::systemapi *> m_hook_end;
  utils::hook<func_type, core::systemapi *> m_hook_update;
  utils::hook<func_type, core::systemapi *> m_hook_render;

  sf::RenderWindow m_window;
  sf::View m_camera;
  flow m_flow;
  assetmanager m_assets;
  ginseng::database m_db;
};
}// namespace core

module :private;
namespace core {


using func_type = game::func_type;
game::game()
  : m_window(sf::VideoMode{ win_width, win_height }, "Animations"),
    m_flow(game::flow::running), m_assets("./resources"), m_db()
{
  m_window.setFramerateLimit(60);
  m_window.setKeyRepeatEnabled(false);
  if (not ImGui::SFML::Init(m_window))
    throw std::runtime_error{ "could not initialize ImGui" };
}

game &game::add_setup_callback(func_type func)
{
  m_hook_setup.connect(func);
  return *this;
}

game &game::add_end_callback(func_type func)
{
  m_hook_end.connect(func);
  return *this;
}

game &game::add_update_callback(func_type func)
{
  m_hook_update.connect(func);
  return *this;
}
game &game::add_render_callback(func_type func)
{
  m_hook_render.connect(func);
  return *this;
}
game &game::add_event_callback(event_func_type func)
{
  m_hook_events.connect(func);
  return *this;
}


void game::run()
{
  auto start = utils::gameclock::now();
  m_flow = flow::running;
  auto game_entity = m_db.create_entity();
  m_db.add_component(game_entity, components::game_tag{});
  m_db.add_component(game_entity, &m_window);
  m_db.add_component(game_entity, m_assets);
  m_db.add_component(game_entity, m_flow);
  m_hook_setup.publish(this);
  say::info(
    fmt::format("Time for startup {} seconds", utils::elapsed_seconds(start)));
  start = utils::gameclock::now();
  sf::Event ev;
  while (m_flow == flow::running && m_window.isOpen())
  {
    float delta = utils::elapsed_seconds(start);
    m_db.add_component(game_entity, delta);
    start = utils::gameclock::now();
    while (m_window.pollEvent(ev))
    {
      ImGui::SFML::ProcessEvent(m_window, ev);
      if (ev.type == sf::Event::Closed) m_flow = flow::stop;
      m_hook_events.publish(this, ev);
    }
    ImGui::SFML::Update(m_window, sf::seconds(delta));
    m_hook_update.publish(this);
    ImGui::Begin("Debug controls");
    ImGui::BeginTabBar("#debug");
    m_hook_render.publish(this);
    ImGui::EndTabBar();
    ImGui::End();
    ImGui::SFML::Render(m_window);
    m_window.display();
  }
  teardown();
}
void game::teardown()
{
  m_hook_end.publish(this);
  ImGui::SFML::Shutdown();
  m_window.close();
}
}// namespace core
