#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Sorting.h"


inline void Draw(std::vector<sf::RectangleShape>& arr , sf::RenderWindow& wd)
{
	for (const auto& n : arr)
	{
		wd.draw(n);
	}
}
int main()
{
	//sortingData sData;
	//selectionData sData;
	bubbleSortData sData;
	int Xpos = 100; float Ypos = 900;
	int count = 0; float baseX = 100.f;
	std::vector<float> exampleArr = { 12.f ,7.f ,8.f, 5.f , 3.f };
	sf::RenderWindow wd(sf::VideoMode(1920, 1080), "test");
	sf::Clock clock;
	sf::Time stepDelay = sf::milliseconds(100);
	sf::Time accumulator = sf::Time::Zero;
	std::vector<sf::RectangleShape> exampleRec = {};
	for (auto i : exampleArr)
	{
		count += 1;
		//figure out waht count is doing 

		sf::RectangleShape tempRec(sf::Vector2f(100.f, -i*100));
		tempRec.setOutlineThickness(1.f);
		tempRec.setPosition(sf::Vector2f(Xpos*count , Ypos));
		tempRec.setFillColor(sf::Color::Green);
		tempRec.setOutlineColor(sf::Color::Red);
		exampleRec.push_back(tempRec);
		std::cout << "Rectangle Shape:" << count << " pushed into ExampleRec" << std::endl;
	
	}
	while (wd.isOpen())
	{
		sf::Event event;
		while (wd.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				wd.close();
			}
		}
		accumulator += clock.restart();

		if (accumulator >= stepDelay)
		{
			/*insertionSort::sortingStep(sData, exampleRec, baseX, Ypos);*/
			/*selectionSort::selectionStep(sData, exampleRec, baseX, Ypos);*/
			bubbleSort::bubbleSortStep(sData, exampleRec, baseX, Ypos);
			accumulator = sf::Time::Zero;
		}
		wd.clear(sf::Color::Black);
		Draw(exampleRec, wd);
		std::cout << "Draw" << std::endl;

		wd.display();
	}
}





