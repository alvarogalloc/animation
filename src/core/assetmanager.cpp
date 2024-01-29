module;
#include <filesystem>
#include <future>
#include <memory>
#include <string_view>
#include <type_traits>
#include <unordered_map>
export module core.assetmanager;
import core.say;
import ext.sfml;

export namespace core {

class assetmanager
{
  template<typename T> using ptr = std::shared_ptr<T>;
  template<typename T>
  using hash_map = std::unordered_map<std::string_view, ptr<T>>;

private:
  // returns true if loaded successfully
  template<typename T> bool load(std::string_view path, hash_map<T> &target_map)
  {
    bool error = false;
    // get type of the map
    auto [res, ok] = target_map.insert({ path, std::make_shared<T>() });
    error = !ok;
    if constexpr (std::is_same_v<T, sf::Music>)
      error = !res->second->openFromFile(asset_path + path.data());
    else
      error = !res->second->loadFromFile(asset_path + path.data());
    return !error;
  }

public:
  explicit assetmanager(std::string_view _asset_path) : asset_path(_asset_path)
  {
    namespace fs = std::filesystem;
    if (!fs::exists(asset_path)) { say::error("Path does not exist"); }
    if (!asset_path.ends_with('/')) { asset_path += '/'; }
  }

  template<typename T> [[nodiscard]] ptr<T> get(std::string_view path)
  {
    static_assert(std::is_same_v<T, sf::Texture> || std::is_same_v<T, sf::Music>
                    || std::is_same_v<T, sf::Font>,
      "Asset Type is not supported");
    // get container to pass to "load"
    hash_map<T> *container{ nullptr };
    if constexpr (std::is_same_v<T, sf::Music>)
      container = &songs;
    else if constexpr (std::is_same_v<T, sf::Font>)
      container = &fonts;
    else if constexpr (std::is_same_v<T, sf::Texture>)
      container = &textures;

    if (!container) say::error("Asset Type is not supported");

    if (!container->contains(path))
    {
      say::debug("asset not loaded, loading");
      if (!load(path, *container)) say::error("error loading asset");
    } else
    {
      say::debug("asset already loaded, returning");
    }

    return container->at(path);
  }
  template<typename T>
  [[nodiscard]] std::future<ptr<T>> async_get(std::string_view path)
  {
    return std::async(std::launch::async, &assetmanager::get<T>, this, path);
  }

private:
  std::string asset_path;
  hash_map<sf::Texture> textures;
  hash_map<sf::Music> songs;
  hash_map<sf::Font> fonts;
};
}// namespace core
