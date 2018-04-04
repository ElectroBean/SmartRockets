#include <iostream>
#include "GA.h"
#include <string>
#include <random>


char getRandomGene();
float fitness(int index);

std::string targetString = "lemon lord";
std::string validChars = "abcdefghijklmnopqrstuvwxyz";

GA<char>* geneticAlgorithm;

int main()
{
	srand(time_t(NULL));
	geneticAlgorithm = new GA<char>(10, targetString.length(), getRandomGene, fitness, 0.01f);
	//geneticAlgorithm->NewGeneration();
	for (auto i = geneticAlgorithm->Population.begin(); i != geneticAlgorithm->Population.end(); i++)
	{
		auto temp = *i;
		auto temp2 = temp.Genes;
		for (auto iter = temp2.begin(); iter != temp2.end(); iter++)
		{
			std::cout << *iter;
		}
		std::cout << "\n";
	}

	//int temp;
	//std::cin >> temp;

	//geneticAlgorithm->NewGeneration();
	//for (auto i = geneticAlgorithm->Population.begin(); i != geneticAlgorithm->Population.end(); i++)
	//{
	//	auto temp = *i;
	//	auto temp2 = temp.Genes;
	//	for (auto iter = temp2.begin(); iter != temp2.end(); iter++)
	//	{
	//		std::cout << *iter;
	//	}
	//	std::cout << "\n";
	//}

	system("pause");
	return 0;
}

char getRandomGene()
{
	//int i = rand() % targetString.length() + 1;
	int i = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (validChars.size() - 0)));
	return validChars[i];
}

float fitness(int index)
{
	float score = 0;
	auto it = geneticAlgorithm->Population.begin();
	std::advance(it, index);
	DNA<char> dna = *it;

	for (int i = 0; i < dna.Genes.size(); i++)
	{
		for (auto iter = dna.Genes.begin(); iter != dna.Genes.end(); iter++)
		{
			if (*iter == targetString[i])
			{
				score += 1;
			}
		}
	}

	score /= targetString.size();

	return score;
}