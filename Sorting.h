#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

struct sortingData
{
	int i = 1;int j = 1;
	sf::Vector2f key;
	bool inserting = false;
};
class Sorting
{
	virtual void setUp();
	
};



class insertionSort : public Sorting
{
private:
	
	//static int m_i ; static int m_j ;
	//static sf::Vector2f m_key;
	//static bool m_inserting ;
public:
	
	static void sortingStep(sortingData& s, std::vector<sf::RectangleShape>& arr, float& baseX, float& Ypos)
	{
		{

			if (s.i >= arr.size()) return;
			if (!s.inserting)
			{
				s.key = arr[s.i].getPosition();
				s.j = s.i - 1;
				s.inserting = true;
				return;
			}
			if (s.j >= 0 && arr[s.j].getSize().y < arr[s.j + 1].getSize().y)

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
	}
};