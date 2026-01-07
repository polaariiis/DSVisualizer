#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>


class Sorting
{
private:
	int m_Xpos; int m_Ypos;
	sf::Clock performanceClock;
public:
	float memoryComplexity;
	float timeComplexity;
public:
	static void m_Buble(std::vector<int>& g_Arr);
	static void m_Insertion(std::vector<int>& g_Arr);
	static void m_Selection(std::vector<int>& g_Arr);
	static void m_Quick(std::vector<int>& g_Arr);
	static void m_Merge(std::vector<int>& g_Arr);
	static void m_Heap(std::vector<int>& g_Arr);

public:
	const void p_Drawing(sf::RenderWindow& wd);
	const void p_Animation(sf::RenderWindow& wd);



};