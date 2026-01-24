#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
//graphic engine only includes controlling drawing shapes to avoid multiple duplication as well more readibility in state machine 
// UI -> graphics
class graphicEngine
{
public:
	const std::string g_fontLoc;
	sf::Font g_font;

public:
	void m_beginFrame(sf::RenderWindow& wd) {}; 
	void m_drawFrame(sf::RenderWindow &wd ,sf::RectangleShape& shape) {};
	void m_drawText(sf::RenderWindow& wd, sf::Text text);
	void m_endFrame(sf::RenderWindow& wd) {};

	//later there will be need for drawing circles and shapes , look for other ways like templates so i dont need each drawing method specific for only one shape 
public:
	graphicEngine(const std::string& fontLoc) : g_fontLoc(fontLoc)
	{
		if (g_font.loadFromFile(g_fontLoc))
		{
			std::cout << "Graphics:Font loaded sucessfully!" << std::endl;
		}
		else
		{
			std::cout << "Graphics:Couldn't load font. Check font file path!";
			//later try to fall back to the other font if one is uncessfull
			//in the future maybe ill add function so that user can choose their own font
			//that time instead of putting it into constructor will have to create seperate method to choose and put different font
			//graphic engine will handle it
		}
	}
};
