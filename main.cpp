#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
	//intialize
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	
	
	sf::RenderWindow wd(sf::VideoMode(800, 600), "smth" , sf::Style::Default , settings);
	sf::CircleShape shape(50.f);
	shape.setFillColor(sf::Color::Red);
	

	//main game loop
	while (wd.isOpen())
	{
		//update
	sf::Event event;
	while (wd.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			wd.close();
		}
		}


	//draw()
	wd.clear(sf::Color::Black);
	wd.draw(shape);
	wd.display();
	
	}
	
}
