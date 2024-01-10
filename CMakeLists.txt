cmake_minimum_required(VERSION 3.28)
project(learning-animation)
set(CMAKE_CXX_STANDARD 20)
include(cmake/setup.cmake)

# generate find_package for thesefind_package("entt", "sfml", "fmt", "box2d")

find_package(EnTT CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
# imgui and imgui-sfml
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
set(MODULE_FILES 
"src/core/animation.cpp"
"src/core/assetmanager.cpp"
"src/core/components.cpp"
"src/core/physics_debug_draw.cpp"
"src/core/physics.cpp"
"src/core/render.cpp"
"src/core/say.cpp"
"src/core/tilemap.cpp"
"src/ext/box2d.cpp"
"src/ext/ginseng.cpp"
"src/ext/sfml.cpp"
"src/game.cpp"
"src/knight.cpp"
)

add_executable(game "src/main.cpp")
target_compile_features(game PRIVATE cxx_std_20)
target_sources(game PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES
${MODULE_FILES})

file(COPY "resources" DESTINATION ${CMAKE_BINARY_DIR})

target_link_libraries(game PRIVATE EnTT::EnTT)
target_link_libraries(game PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
target_link_libraries(game PRIVATE fmt::fmt)
target_link_libraries(game PRIVATE box2d::box2d)
target_link_libraries(game PRIVATE imgui::imgui)
target_link_libraries(game PRIVATE ImGui-SFML::ImGui-SFML)
target_link_libraries(game PRIVATE rooster_project_options rooster_project_warnings)
# remove warning C4251 in msvc
target_compile_options(game PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/wd4251>")
