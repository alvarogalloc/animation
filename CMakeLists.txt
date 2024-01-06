cmake_minimum_required(VERSION 3.28)
project(learning-animation)
set(CMAKE_CXX_STANDARD 23)

# generate find_package for thesefind_package("entt", "sfml", "fmt", "box2d")

find_package(EnTT CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
set(MODULE_FILES 
    src/core/animation.mpp
    src/core/assetmanager.mpp
    src/core/components.mpp
    src/core/physics.mpp
    src/core/render.mpp
    src/core/say.mpp
    src/core/tilemap.mpp
    src/ext/ginseng.mpp
    src/ext/sfml.mpp
    src/game.mpp
    src/knight.mpp
)

add_executable(game src/main.cpp)
target_compile_features(game PRIVATE cxx_std_23)
target_sources(game PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES
${MODULE_FILES})


target_link_libraries(game PRIVATE EnTT::EnTT)
target_link_libraries(game PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
target_link_libraries(game PRIVATE fmt::fmt)
target_link_libraries(game PRIVATE box2d::box2d)