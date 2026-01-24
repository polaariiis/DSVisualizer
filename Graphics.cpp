#include "Graphics.h"

void graphicEngine::m_beginFrame(sf::RenderWindow& wd)
{
	wd.clear(sf::Color::Black);
}

void graphicEngine::m_drawFrame(sf::RenderWindow& wd  , sf::RectangleShape& shape)
{
	wd.draw(shape);
}

void graphicEngine::m_endFrame(sf::RenderWindow& wd)
{
	wd.display();
}
void graphicEngine::m_drawText(sf::RenderWindow& wd, sf::Text& text)
{
	wd.draw(text);
}
