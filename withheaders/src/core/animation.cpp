#include <core/animation.hpp>



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
