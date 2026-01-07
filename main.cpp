#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>


void BubbleSort(std::vector<int>& Arr)
{
	int sizeArr = Arr.size();
	for (int i = 1; i < sizeArr; i++)
	{
		int key = Arr[i];
		int j = i - 1;

		while (j >= 0 && Arr[j] > key)
		{
			Arr[j + 1] = Arr[j];
			j = j - 1;
		}
		Arr[j + 1] = key;
	}
}

int main()
{
	std::vector<int> exampleArr = { 3 ,2 ,1, 6 , 15 };
	sf::RenderWindow wd(sf::VideoMode(1920, 1080), "test");







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



		wd.clear(sf::Color::Black);
		wd.display();
	}
	BubbleSort(exampleArr);
	for (int i : exampleArr)
	{
		std::cout << i << " ";
	}


}





