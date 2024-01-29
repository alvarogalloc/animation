module;
#include <string_view>
#include <unordered_map>
export module core.animation;
import ext.ginseng;
import ext.sfml;
import core.components;

export namespace core::components {
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

export namespace core::systems {
void update_animations(ginseng::database &db);
}// namespace core::systems

module :private;

namespace core::systems {

void update_animations(ginseng::database &db)
{
  db.visit([&](float &delta) {
    auto update_animation = [&](components::sprite &sprite,
                              components::animation &anim) {
      const auto frame_time = anim.duration / anim.n_frames;
      anim.passed_time += delta;
      const auto [w, h] = sprite.getTexture()->getSize();
      const auto frame_size_x = w / anim.n_frames;
      if (anim.passed_time >= anim.duration)
      {
        anim.current_frame = 0;
        anim.passed_time = 0;
        sprite.setTextureRect(
          { static_cast<int>(frame_size_x * anim.current_frame)
              + anim.actual_size.left,
            anim.actual_size.top,
            anim.actual_size.width,
            anim.actual_size.height });
      }
      if (anim.passed_time - (frame_time * anim.current_frame) >= frame_time)
      {
        anim.current_frame++;
        sprite.setTextureRect(
          { static_cast<int>(frame_size_x * anim.current_frame)
              + anim.actual_size.left,
            anim.actual_size.top,
            anim.actual_size.width,
            anim.actual_size.height });
      }
    };

    db.visit([&](components::sprite &sprite, components::animations &anims) {
      if (!anims.playing) return;
      auto &current_sequence = anims.sequences[anims.current_sequence];

      sprite.setTexture(*(current_sequence.texture));
      update_animation(sprite, current_sequence);
    });
  });
}
}// namespace core::systems
