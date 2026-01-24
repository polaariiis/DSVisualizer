#pragma once
#include <SFML/Graphics.hpp>
class Button
{
private:
public:
	sf::RectangleShape shape;
	sf::Text text;
public:
	Button(sf::Vector2f size, sf::Vector2f pos, sf::Font& font, std::string label, int charSize)
	{
		shape.setSize(size);
		shape.setPosition(pos);
		shape.setFillColor(sf::Color::Blue);

		text.setFont(font);
		text.setString(label);
		text.setCharacterSize(charSize);
		text.setFillColor(sf::Color::White);

		sf::FloatRect textRect = text.getGlobalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		text.setPosition(pos.x + size.x / 2, pos.y + size.y / 2);
	}
	bool isClicked(sf::Vector2f mousePos)
	{
		return shape.getGlobalBounds().contains(mousePos);
	}
};

class Menu
{
public:
	std::string fontPath = "C:/Users/USER/source/repos/newlearning/font.ttf";
public:
	Menu()
	{
		Button startingButton();

	}
};