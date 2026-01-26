#include <SFML/Graphics.hpp>
#include <iostream>

#include "StateManager.h"
#include "States.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "DS Visualizer v0.1", sf::Style::Close);
    window.setFramerateLimit(120);

    sf::Font font;
    if (!font.loadFromFile("font.ttf"))
    {
        std::cerr << "Failed to load font.ttf (make sure it's next to the exe).\n";
        return 1;
    }

    StateManager sm;
    sm.set(std::make_unique<MainMenuState>(sm, window, font));

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

            sm.handleEvent(e);
            sm.applyPending();   // safe after events
        }

        float dt = clock.restart().asSeconds(); // <-- THIS was missing

        sm.update(dt);
        sm.applyPending();       // also safe here

        window.clear();
        sm.render(window);
        window.display();
    }


    return 0;
}