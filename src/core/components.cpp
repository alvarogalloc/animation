export module core.components;
import ext.sfml;
import ext.ginseng;

export namespace core::components {

struct sprite : public sf::Sprite
{
};
struct color : public sf::Color
{
};
using game_tag = ginseng::tag<struct game_tag_t>;


}// namespace core::components
