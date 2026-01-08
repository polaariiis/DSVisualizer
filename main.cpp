#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>


//void BubbleSort(std::vector<int>& Arr)
//{
//	int sizeArr = Arr.size();
//	for (int i = 1; i < sizeArr; i++)
//	{
//		int key = Arr[i];
//		int j = i - 1;
//
//		while (j >= 0 && Arr[j] > key)
//		{
//			Arr[j + 1] = Arr[j];
//			j = j - 1;
//		}
//		Arr[j + 1] = key;
//	}
//}

void Draw(std::vector<sf::RectangleShape>& arr , sf::RenderWindow& wd)
{
	for (const auto& n : arr)
	{
		wd.draw(n);

	}

}
//figure out way to use Draw instead of passing it into BubbleSort too costly
struct sortingData
{
	int i = 1;int j = 1;
	sf::Vector2f key;
	bool inserting = false;
};
void sortingStep(sortingData& s, std::vector<sf::RectangleShape>& arr, float& baseX , float& Ypos)
{

	if (s.i >= arr.size()) return;
	if (!s.inserting)
	{
		s.key = arr[s.i].getPosition();
		s.j = s.i - 1;
		s.inserting = true;
		return;
	}
	if ( s.j >= 0 && arr[s.j].getSize().y < arr[s.j + 1].getSize().y)

	{
		std::swap(arr[s.j], arr[s.j + 1]);
		s.j--;

		for (int k = 0; k < arr.size(); k++)
		{
			arr[k].setPosition(baseX * (k + 1), Ypos);
		}
	}
	else
	{

		s.i++;
		s.inserting = false;
	}



}
int main()
{
	sortingData sData;
	int Xpos = 100; float Ypos = 900;
	int count = 0; float baseX = 100.f;


	std::vector<float> exampleArr = { 3.f ,2.f ,1.f, 6.f , 8.f };
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
			sortingStep(sData , exampleRec , baseX , Ypos);
			accumulator = sf::Time::Zero;
		}
		wd.clear(sf::Color::Black);
		Draw(exampleRec, wd);
		std::cout << "Draw" << std::endl;

		wd.display();
	}
	//BubbleSort(exampleArr);
	//for (int i : exampleArr)
	//{
	//	std::cout << i << " ";
	//}


}





