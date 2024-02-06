#pragme once
#include <string_view>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <ginseng.hpp>


namespace core::components {
struct animation
{
  /*
   * the logic with this will
   * be:
   * - the texture width will be divided
   *   by n_frames
   * - with that, a rect will provide
   *   the actual rect of a sprite from
   *   the texture trimmed rect
   *   */
  std::uint8_t n_frames;
  sf::Texture *texture = nullptr;
  float duration;
  sf::IntRect actual_size;
  std::uint8_t current_frame{ 0 };
  float passed_time{ 0.f };
  // this is to get narrower tiles instead of 120 *120 tiles or so
};
struct animations
{
  std::string_view current_sequence;
  std::unordered_map<std::string_view, animation> sequences{};
  bool playing{ true };
};
}// namespace core::components

namespace core::systems {
void update_animations(ginseng::database &db);
}// namespace core::systems

