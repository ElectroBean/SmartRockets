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

	std::list<T> bestGenes;
	float bestFitness;

	GA(int populationSize, 
		int dnaSize, 
		std::function<T()> getRandomGene, 
		std::function<float(int)>  fitnessFunction, 
		float mutationRate = 0.1f)
	{
		this->dnaSize = dnaSize;
		this->getRandomGene = getRandomGene;
		this->fitnessFunction = fitnessFunction;
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
		generationNumber++;
		//make sure the population has at least 1 member
		if (Population.size() <= 0)
		{
			return;
		}
		//calculate fitness of all members of population
		CalculateFitness();
		
		//initialize new population
		std::list<DNA<T>> newPopulation = std::list<DNA<T>>();

		//problem is in here somewhere, not anymore lmao
		for (int i = 0; i < Population.size(); i++)
		{
			//problem not in chooseparent or not
			DNA<T> parent1 = ChooseParent();
			DNA<T> parent2 = ChooseParent();

			
			//problem might be in crossover, defs not
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
			sumFitness += (*iter).CalculateFitness(index);

			if ((*iter).fitness > bestFitness)
			{
				bestFitness = (*iter).fitness;
				bestGenes = temp.Genes;
			}
			//sumFitness += *iter.CalculateFitness(iter);
		}

		//bestFitness = best.fitness;
		//bestGenes = best.Genes;
	}

	DNA<T> ChooseParent()
	{
		float randomNumber = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * sumFitness;
		//for (int i = 0; i < Population.size(); i++)
		//{
		//	if (randomNumber < Population[i].fitness)
		//	{
		//		return Population[i];
		//	}
		//
		//	randomNumber -= Population[i]->fitness;
		//}


		float randChoice = rand() % 100 + 1;
		//50% elitist 50% higher fitness has higher chance to be chosen as parent
		//if (randChoice < 50)
		//{
			//sort population by fitness
			Population.sort([](DNA<T>  lhs, DNA<T>  rhs) {return lhs.fitness > rhs.fitness; });
			//get random number equal to 25% of size of population
			int randomN = Population.size() * 0.25f;
			//choose a random iterator number from 0 to the random number we just got
			int randPlace = rand() % randomN + 0;
			//set iterator to begin of population
			auto iter = Population.begin();
			//iterate to random iterator number place
			std::advance(iter, randPlace);
			//return DNA at iterator position
			return *iter;
		//}
		//else
		//{


			//}
			//else
			//{
			//	//for each member in the population
			//	for (auto iter = Population.begin(); iter != Population.end(); iter++)
			//	{
			//		//set temp variable to current iterator
			//		DNA<T> temp = *iter;
			//		//if our random number with fitness is less than our temps DNAs fitness
			//		if (randomNumber < temp.fitness)
			//		{
			//			//return our temp DNA
			//			return temp;
			//		}
			//
			//		//take temp fitness from our random number
			//		randomNumber -= temp.fitness;
			//	}
			//}
			//Default return will be our best genes
			//because I currently dont know how to set it to null without breaking everything
			DNA<T> newDNA = DNA<T>(dnaSize, getRandomGene, fitnessFunction);
			newDNA.Genes = bestGenes;
			return newDNA;
		//}
	}

private:
	int dnaSize;
	std::function<T()> getRandomGene;
	std::function<float(int)>  fitnessFunction;

};

//while (hasntReached)
//{
//	geneticAlgorithm->NewGeneration();
//	for (auto i = geneticAlgorithm->Population.begin(); i != geneticAlgorithm->Population.end(); i++)
//	{
//		auto temp = *i;
//		auto temp2 = temp.Genes;
//		for (auto iter = temp2.begin(); iter != temp2.end(); iter++)
//		{
//			std::cout << *iter;
//		}
//
//		if ((*i).fitness == 6)
//		{
//			hasntReached = false;
//		}
//		std::cout << "\n";
//	}
//
//	std::cout << "\n New Generation \n";
//}