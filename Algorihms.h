#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

struct SortMetrics {
	size_t comparisons = 0;
	size_t swaps = 0;
};
struct bubbleSortData
{
	int i = 0;  int j = 1;
	bool swapped = 1;
	bool swapping = 0;
};
struct sortingData
{
	int i = 1;int j = 1;
	sf::Vector2f key;
	bool inserting = false;
};
struct selectionData
{
	int i = 0; int j = 1;
	int minIdx = 0;
	bool selecting = false;
	bool minRecChange = false;
};
class Sorting
{
public:
	static void bubbleSortStep(bubbleSortData& s,
		std::vector<sf::RectangleShape>& arr,
		float baseX, float Ypos)
	{
		if (s.i >= arr.size() - 1)
			return;

		if (s.j >= arr.size() - s.i - 1)
		{
			// end of one pass
			if (!s.swapped)
				return; // already sorted

			s.j = 0;
			s.swapped = false;
			s.i++;
			return;
		}

		if (arr[s.j].getSize().y < arr[s.j + 1].getSize().y)
		{
			std::swap(arr[s.j], arr[s.j + 1]);
			s.swapped = true;

			for (int k = 0; k < arr.size(); k++)
				arr[k].setPosition(baseX * (k + 1), Ypos);
		}

		s.j++;
	}
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
	static void selectionStep(selectionData& s,
		std::vector<sf::RectangleShape>& arr,
		float baseX,
		float Ypos)
	{
		if (s.i >= arr.size() - 1) return;


		if (!s.selecting)
		{
			s.minIdx = s.i;
			s.j = s.i + 1;
			s.selecting = true;
			return;
		}

		if (s.j < arr.size())
		{
			if (arr[s.j].getSize().y < arr[s.minIdx].getSize().y)
			{
				s.minIdx = s.j;
			}
			s.j++;
			return;
		}


		if (s.minIdx != s.i)
		{
			std::swap(arr[s.i], arr[s.minIdx]);

			for (int k = 0; k < arr.size(); k++)
				arr[k].setPosition(baseX * (k + 1), Ypos);
		}

		s.i++;
		s.selecting = false;
	}
};
