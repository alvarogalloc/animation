module;
#include <chrono>
#include <thread>
export module core.utils;

export namespace core::utils {

template<typename Sig, typename... Args> class hook
{
public:
  using callback_type = Sig;
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

private:
  std::vector<callback_type> m_callbacks;
};

using gameclock = std::chrono::steady_clock;
float elapsed_seconds(gameclock::time_point start);

void sleep_ms(std::uint32_t ms);
}// namespace core::utils


module :private;
namespace core::utils {

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

}// namespace core::utils
