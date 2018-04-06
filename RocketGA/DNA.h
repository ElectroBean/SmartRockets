#pragma once
#include <list>
#include <iostream>
#include <functional>
#include <random>
#include <time.h>

template<typename T>
class DNA
{
public:

	std::list<T> Genes;
	float fitness;

	DNA(int size, std::function<T()> getRandomGene, std::function<float(int)> fitnessFunction, bool shouldInitGenes = true)
	{
		Genes = std::list<T>(size);
		this->getRandomGene = getRandomGene;
		this->fitnessFunction = fitnessFunction;

		//for (int i = 0; i < Genes.size(); i++)
		//{
		//	//set random genes of gene array
		//	Genes[i] = this->getRandomGene();
		//}

		if (shouldInitGenes)
		{
			for (auto iter = Genes.begin(); iter != Genes.end(); iter++)
			{
				*iter = this->getRandomGene();
			}

		}
	}

	~DNA()
	{

	}

	float CalculateFitness(int index)
	{
		this->fitness = fitnessFunction(index);
		return this->fitness;
	}

	DNA<T> Crossover(DNA<T> otherParent)
	{
		DNA<T> child = DNA<T>(Genes.size(), getRandomGene, fitnessFunction, false);

		//for (int i = 0; i < Genes.size(); i++)
		//{
		//	//do random setting of genes
		//	float randomNumber = rand() % 1 + 0;
		//	child.Genes[i] = randomNumber < 0.5f ? Genes[i] : otherParent.Genes[i];
		//}

		//setting gene to single parents gene
		//results in repeating genes on all occasions
		//need to implement proper crossover
		for (auto iter = child.Genes.begin(); iter != child.Genes.end(); iter ++)
		{
			//for (auto iter2 = Genes.begin(); iter2 != Genes.end(); iter2++)
			//{
			//	*iter = *iter2;
			//	break;
			//}
			int difference = std::distance(child.Genes.begin(), iter);
			auto parent1Iter = Genes.begin();
			std::advance(parent1Iter, difference);
			auto parent2Iter = otherParent.Genes.begin();
			std::advance(parent2Iter, difference);

			//random float between 0 and 1 hopefully
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			//*iter = randomNumber < 0.5f ? *parent1Iter : *parent2Iter;
			if (r < 0.5f)
			{
				*iter = *parent1Iter;
			}
			else
			{
				*iter = *parent2Iter;
			}
		}

		return child;
	}

	void Mutate(float mutationRate)
	{
		//for (int i = 0; i < Genes.size(); i++)
		//{
		//	float randomNumber = rand() % 100 + 1;
		//	//if check random number is less than mutation rate
		//	if (randomNumber < mutationRate)
		//	{
		//		//set gene at i to new random gene
		//		Genes[i] = getRandomGene();
		//	}
		//}

		for (auto iter = Genes.begin(); iter != Genes.end(); iter++)
		{
			float randomNumber = rand() % 100 + 1;
			if (randomNumber < mutationRate)
			{
				*iter = getRandomGene();
			}
		}
	}

private:

	std::function<T()> getRandomGene;
	std::function<float(int)> fitnessFunction;

};

