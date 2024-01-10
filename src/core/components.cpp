export module components;
import sfml;
import ginseng;

export namespace components {

struct sprite : public sf::Sprite
{
};
struct color : public sf::Color
{
};
using render_box = ginseng::tag<struct render_box_tag>;
using game_tag = ginseng::tag<struct game_tag_t>;


}// namespace components
