#include <iostream>
#include "GA.h"
#include <string>
#include <random>
#include <ctgmath>


char getRandomGene();
float fitness(int index);
int randomgen(int max, int min);

std::string targetString = "if jesus could walk on water, could he swim on land?";
std::string validChars = "abcdefghijklmnopqrstuvwxyz .,?";

GA<char>* geneticAlgorithm;

bool hasntReached = true;



int main()
{
	srand(time(0));
	geneticAlgorithm = new GA<char>(10, targetString.length(), getRandomGene, fitness, 2.5f);
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


	while (hasntReached)
	{
		if (geneticAlgorithm->bestFitness == 1)
		{
			std::cout << "\n Best genes were\n";
			for (auto iter = geneticAlgorithm->bestGenes.begin(); iter != geneticAlgorithm->bestGenes.end(); iter++)
			{
				std::cout << *iter;
			}
			std::cout << "\n Generations: " << geneticAlgorithm->generationNumber << std::endl;
			hasntReached = false;
			break;
		}
		geneticAlgorithm->NewGeneration();
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
	
		std::cout << "\n New Generation \n";
	}

	//int temp;
	//std::cin >> temp;
	//
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

	delete geneticAlgorithm;
	system("pause");
	return 0;
}

char getRandomGene()
{
	int random = rand() % validChars.size() + 0;
	return validChars[random];
}

float fitness(int index)
{
	float score = 0;
	auto it = geneticAlgorithm->Population.begin();
	std::advance(it, index);
	DNA<char> dna = *it;

	for (int i = 0; i < dna.Genes.size(); i++)
	{
		auto iter = dna.Genes.begin();
		std::advance(iter, i);
		if (*iter == targetString[i])
		{
			score += 1;
		}
		//for (auto iter = dna.Genes.begin(); iter != dna.Genes.end(); iter++)
		//{
		//	if (*iter == targetString[i])
		//	{
		//		score += 1;
		//	}
		//}
	}

	score /= targetString.size();
	score = (pow(5, score) - 1) / (5 - 1);
	return score;
}

int randomgen(int max, int min) //Pass in range
{
	srand(time(NULL));  //Changed from rand(). srand() seeds rand for you.

	int random = rand() % max + min;
	std::cout << random << std::endl;
	return random;

}