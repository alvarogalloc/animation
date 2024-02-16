export module core.systemapi;
import ext.sfml;
import ext.ginseng;
import core.assetmanager;

export namespace core {
  // this is a class that will be passed to the systems allowing them to access
  // * assetmanager
  // * database of entities
  // * window
  // * camera
  class systemapi
  {
    public:
      virtual sf::RenderWindow &window() = 0;
      virtual sf::View &camera() = 0;
      virtual assetmanager &assets() = 0;
      virtual ginseng::database &database() = 0;

      // const accessors
      virtual const sf::RenderWindow &window() const = 0;
      virtual const sf::View &camera() const = 0;
      virtual const assetmanager &assets() const = 0;
      virtual const ginseng::database &database() const = 0;

      virtual ~systemapi() = default;
  };
} // namespace core