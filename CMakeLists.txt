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

# make a library for the engine
add_library(engine STATIC ${MODULE_FILES})
target_compile_features(engine PUBLIC cxx_std_23)
target_link_libraries(engine PUBLIC EnTT::EnTT)
target_link_libraries(engine PUBLIC sfml-system sfml-network sfml-graphics sfml-window)
target_link_libraries(engine PUBLIC fmt::fmt)
target_link_libraries(engine PUBLIC box2d::box2d)
target_link_libraries(engine PUBLIC imgui::imgui)
target_link_libraries(engine PUBLIC ImGui-SFML::ImGui-SFML)
target_link_libraries(engine PUBLIC rooster_project_options rooster_project_warnings)
target_compile_options(engine PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:/wd4251>")
target_sources(engine PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES
${MODULE_FILES})

add_executable(game "src/main.cpp")
target_link_libraries(game PRIVATE engine)
file(COPY "resources" DESTINATION ${CMAKE_BINARY_DIR})

# if option TESTS is enabled
# add test for each file in test folder
option(BUILD_TESTING "Enable Test Builds" OFF)
if(BUILD_TESTING)
    find_package(ut CONFIG REQUIRED)
    enable_testing()
    file(GLOB TEST_SOURCES "test/*.cpp")
    foreach(TEST_SOURCE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
        add_executable(${TEST_NAME} ${TEST_SOURCE})
        target_link_libraries(${TEST_NAME} PRIVATE engine Boost::ut )
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()
endif()