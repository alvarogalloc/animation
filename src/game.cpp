module;
#include <chrono>
#include <functional>
#include <thread>
#include <imgui.h>
#include <imgui-SFML.h>
export module game;
import assetmanager;
import components;
import say;
import ginseng;
import sfml;

template<typename Sig, typename... Args> class hook
{
private:
  using callback_type = Sig;
  std::vector<callback_type> m_callbacks;

public:
  void connect(callback_type callback, bool front = false)
  {
    if (front)
      m_callbacks.insert(m_callbacks.begin(), callback);
    else
      m_callbacks.push_back(callback);
  }

  void publish(Args... args)
  {
    for (auto callback : m_callbacks) callback(args...);
  }
};
using gameclock = std::chrono::steady_clock;
float elapsed_seconds(gameclock::time_point start)
{
  // this is duration<float, 1> which means
  // seconds represented in float
  auto duration = std::chrono::duration<float>(gameclock::now() - start);
  return duration.count();
}
void sleep_ms(std::uint32_t ms)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

export
{
  // ideal for 60 frames per second
  constexpr std::chrono::milliseconds frame_time(16);
  constexpr std::uint32_t win_width{ 1280 };
  constexpr std::uint32_t win_height{ 720 };

  class game
  {

  public:
    using func_type = std::function<void(ginseng::database &)>;
    enum class flow { running, stop };

    game();

    game &add_setup_callback(func_type func);
    game &add_end_callback(func_type func);
    game &add_update_callback(func_type func);
    game &add_render_callback(func_type func);

    void run();


  private:
    hook<func_type, ginseng::database &> m_hook_setup;
    hook<func_type, ginseng::database &> m_hook_end;
    hook<func_type, ginseng::database &> m_hook_update;
    hook<func_type, ginseng::database &> m_hook_render;

    sf::RenderWindow m_window;
    flow m_flow;
    assetmanager m_assets;
    ginseng::database m_db;
  };
}

module :private;

using func_type = game::func_type;
game::game()
  : m_window(sf::VideoMode{ win_width, win_height }, "Animations"),
    m_flow(game::flow::running), m_assets("./resources"), m_db()
{
  m_window.setFramerateLimit(60);
  ImGui::SFML::Init(m_window);
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


void game::run()
{
  auto start = gameclock::now();
  m_flow = flow::running;
  auto game_entity = m_db.create_entity();
  m_db.add_component(game_entity, components::game_tag{});
  m_db.add_component(game_entity, &m_window);
  m_db.add_component(game_entity, m_assets);
  m_db.add_component(game_entity, m_flow);
  m_hook_setup.publish(m_db);
  say::info(fmt::format("Time for startup {} seconds", elapsed_seconds(start)));
  start = gameclock::now();
  sf::Event ev;
  while (m_flow == flow::running && m_window.isOpen())
  {
    float delta = elapsed_seconds(start);
    m_db.add_component(game_entity, delta);
    start = gameclock::now();
    while (m_window.pollEvent(ev))
    {
      ImGui::SFML::ProcessEvent(m_window, ev);
      if (ev.type == sf::Event::Closed) m_flow = flow::stop;
    }
    ImGui::SFML::Update(m_window, sf::seconds(delta));
    m_hook_update.publish(m_db);
    m_hook_render.publish(m_db);
    ImGui::SFML::Render(m_window);
    m_window.display();
  }
  m_hook_end.publish(m_db);
  m_window.close();
}
