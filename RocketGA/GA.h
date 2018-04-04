#pragma once
#include "DNA.h"
#include <list>
#include <iostream>
#include <functional>
#include <random>
#include <time.h>

template<typename T>
class GA
{
public:

	std::list<DNA<T>> Population;
	int generationNumber;
	float mutationRate;
	float sumFitness;

	//T bestGenes[1];
	//float bestFitness;

	GA(int populationSize, 
		int dnaSize, 
		std::function<T()> getRandomGene, 
		std::function<float(int)>  fitnessFunction, 
		float mutationRate = 0.1f)
	{
		generationNumber = 1;
		this->mutationRate = mutationRate;
		Population = std::list<DNA<T>>();

		for (int i = 0; i < populationSize; i++)
		{
			Population.push_back(DNA<T>(dnaSize, getRandomGene, fitnessFunction));
		}
	}

	//create new generation of whatevers
	void NewGeneration()
	{
		//make sure the population has at least 1 member
		if (Population.size() <= 0)
		{
			return;
		}
		//calculate fitness of all members of population
		CalculateFitness();
		
		//initialize new population
		std::list<DNA<T>> newPopulation = std::list<DNA<T>>();

		//problem is in here somewhere
		for (int i = 0; i < Population.size(); i++)
		{
			//problem not in chooseparent
			DNA<T> parent1 = ChooseParent();
			DNA<T> parent2 = ChooseParent();
			
			//problem might be in crossover
			DNA<T> child = parent1.Crossover(parent2);
			child.Mutate(mutationRate);
			
			newPopulation.push_back(child);
		}

		Population = newPopulation;
	}

	void CalculateFitness()
	{
		sumFitness = 0;
		//DNA<T> best = Population[0];

		//for (int i = 0; i < Population.size(); i++)
		//{
		//	sumFitness += Population[i]->CalculateFitness(i);
		//
		//	//if (Population[i].fitness > best.fitness)
		//	//{
		//	//	bestFitness = Population[i];
		//	//}
		//}

		for (auto iter = Population.begin(); iter != Population.end(); iter++)
		{
			DNA<T> temp = *iter;
			int index = std::distance(Population.begin(), iter);
			sumFitness += temp.CalculateFitness(index);
			//sumFitness += *iter.CalculateFitness(iter);
		}

		//bestFitness = best.fitness;
		//bestGenes = best.Genes;
	}

	DNA<T> ChooseParent()
	{
		float randomNumber = ((double)rand() / (RAND_MAX));
		//for (int i = 0; i < Population.size(); i++)
		//{
		//	if (randomNumber < Population[i].fitness)
		//	{
		//		return Population[i];
		//	}
		//
		//	randomNumber -= Population[i]->fitness;
		//}
		for (auto iter = Population.begin(); iter != Population.end(); iter++)
		{
			DNA<T> temp = *iter;
			if (randomNumber < temp.fitness)
			{
				return temp;
			}
			
			randomNumber -= temp.fitness;
		}
		return *Population.begin();
	}

private:
};

