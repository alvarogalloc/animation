module;
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
export module ext.sfml;
export namespace sf {
using sf::Clock;
using sf::Time;
using sf::seconds;
using sf::Color;
using sf::Event;
using sf::Font;
using sf::Music;
using sf::FloatRect;
using sf::IntRect;
using sf::Keyboard;
using sf::Mouse;
using sf::RectangleShape;
using sf::CircleShape;
using sf::ConvexShape;
using sf::RenderTarget;
using sf::RenderWindow;
using sf::View;
using sf::Sprite;
using sf::Texture;
using sf::Image;
using sf::Vector2f;
using sf::Vector2i;
using sf::Vector2u;
using sf::VideoMode;
using sf::VertexArray;
using sf::RenderStates;
using sf::Vertex;
using sf::PrimitiveType;
using sf::Transformable;
using sf::Drawable;
using sf::Vector2;
using sf::operator-;
using sf::operator+;
using sf::operator+=;
using sf::operator-=;
using sf::operator-=;
using sf::operator*;
using sf::operator*=;
using sf::operator/;
using sf::operator/=;
using sf::operator==;
using sf::operator!=;
}// namespace sf

export namespace ImGui {
  namespace SFML {
  using ImGui::SFML::Init;
  using ImGui::SFML::SetCurrentWindow;
  using ImGui::SFML::ProcessEvent;
  using ImGui::SFML::Update;
  using ImGui::SFML::Render;
  using ImGui::SFML::Shutdown;
  using ImGui::SFML::UpdateFontTexture;
  using ImGui::SFML::GetFontTexture;
  using ImGui::SFML::SetActiveJoystickId;
  using ImGui::SFML::SetJoystickDPadThreshold;
  using ImGui::SFML::SetJoystickLStickThreshold;
  using ImGui::SFML::SetJoystickRStickThreshold;
  using ImGui::SFML::SetJoystickLTriggerThreshold;
  using ImGui::SFML::SetJoystickRTriggerThreshold;
  using ImGui::SFML::SetJoystickMapping;
  using ImGui::SFML::SetDPadXAxis;
  using ImGui::SFML::SetDPadYAxis;
  using ImGui::SFML::SetLStickXAxis;
  using ImGui::SFML::SetLStickYAxis;
  using ImGui::SFML::SetRStickXAxis;
  using ImGui::SFML::SetRStickYAxis;
  using ImGui::SFML::SetLTriggerAxis;
  using ImGui::SFML::SetRTriggerAxis;
}
  using ImGui::Image;
  using ImGui::ImageButton;
  using ImGui::DrawLine;
  using ImGui::DrawRect;
  using ImGui::DrawRectFilled;
}
