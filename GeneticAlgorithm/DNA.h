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

	DNA<T>* Crossover(DNA<T>* otherParent)
	{
		//might look like a leak, but it isnt, this gets deleted later on
		DNA<T>* child = new DNA<T>(Genes.size(), getRandomGene, fitnessFunction, false);
	
		for (auto iter = child->Genes.begin(); iter != child->Genes.end(); iter++)
		{
			//iter to int 
			int difference = std::distance(child->Genes.begin(), iter);
			//set first parent gene(advance goes to random iterator position)
			auto parent1Iter = Genes.begin();
			std::advance(parent1Iter, difference);
			//set second parent gene(can currently be same as first parent) - (advance goes to random iterator position)
			auto parent2Iter = otherParent->Genes.begin();
			std::advance(parent2Iter, difference);

			//random float between 0 and 1 hopefully
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			//if random number was less than 0.5(arbitrary)
			if (r < 0.5f)
			{
				//set childs current gene to first parents gene
				*iter = *parent1Iter;
			}
			else
			{
				//set childs current gene to second parents gene
				*iter = *parent2Iter;
			}
		}

		//return child
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
			//random number from 1 to 100 inclusive
			float randomNumber = rand() % 100 + 1;
			//if randomnumber is less than mutation rate
			if (randomNumber < mutationRate)
			{
				//set random gene at current position in population
				*iter = getRandomGene();
			}
		}
	}

private:

	std::function<T()> getRandomGene;
	std::function<float(int)> fitnessFunction;
	
};
