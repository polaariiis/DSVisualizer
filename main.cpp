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
	for (auto n : arr)
	{
		wd.draw(n);
	}

}
//figure out way to use Draw instead of passing it into BubbleSort too costly
void InsertionSortDrawing(std::vector<sf::RectangleShape>& arr, sf::RenderWindow& wd )
{

	int sizeArr = arr.size();
	for (int i = 1; arr.size();i++)
	{
		sf::RectangleShape key = arr[i];
		int j = i - 1;
		while (j>=0 && arr[j].getSize().y>key.getSize().y)
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		} 
		// change the logic instead of changing the rectangles change the position of each rectangle and draw at each frame
		int e = j + 1;
		arr[j + 1] = key;
		//here bug exist fix asap
		std::cout << "Looped through BubbleDrawing func for 1 time!" << std::endl;
		Draw(arr , wd);
	
		
	}
}
int main()
{
	int Xpos = 100; int Ypos = 100;
	int count = 0;
	std::vector<int> exampleArr = { 3 ,2 ,1, 6 , 8 };
	sf::RenderWindow wd(sf::VideoMode(1920, 1080), "test");

	std::vector<sf::RectangleShape> exampleRec = {};
	for (auto i : exampleArr)
	{
		count += 1;
		//figure out waht count is doing 

		sf::RectangleShape tempRec(sf::Vector2f(100.f, 100.f * i));
		tempRec.setOutlineThickness(1.f);
		tempRec.setPosition(sf::Vector2f(Xpos * count, Ypos));
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



		wd.clear(sf::Color::Black);
		//better implementation for drawing
		InsertionSortDrawing(exampleRec, wd);
		wd.display();
	}
	//BubbleSort(exampleArr);
	//for (int i : exampleArr)
	//{
	//	std::cout << i << " ";
	//}


}





