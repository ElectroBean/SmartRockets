#pragma once
#include "DNA.h"
#include <list>
#include <iostream>
#include <functional>
#include <random>
#include <time.h>

template<typename T>
class GeneticAlgorithm
{
public:

	std::list<DNA<T>*> Population;
	std::list<DNA<T>*> newPopulation;
	int generationNumber;
	float mutationRate;
	float sumFitness;

	std::list<T> bestGenes;
	float bestFitness;
	int bestGenesGeneration = 1;


	GeneticAlgorithm(int populationSize,
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
		Population = std::list<DNA<T>*>();
		newPopulation = std::list<DNA<T>*>();

		//give population random dna
		for (int i = 0; i < populationSize; i++)
		{
			Population.push_back(new DNA<T>(dnaSize, getRandomGene, fitnessFunction));
		}
	}

	~GeneticAlgorithm()
	{
		//get rid of dem memory leaks
		for (auto iter = Population.begin(); iter != Population.end(); iter++)
		{
			delete *iter;
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
		newPopulation.clear();

		//problem is in here somewhere, not anymore lmao
		for (int i = 0; i < Population.size(); i++)
		{
			//problem not in chooseparent or not
			DNA<T>* parent1 = ChooseParent();
			DNA<T>* parent2 = ChooseParent();


			//problem might be in crossover, defs not
			DNA<T>* child = parent1->Crossover(parent2);
			child->Mutate(mutationRate);

			//*newPopulation->push_back(child);
			newPopulation.push_back(child);
		}


		std::list<DNA<T>*> tempList = Population;

		//delete pointers in old population
		for (auto iter = Population.begin(); iter != Population.end(); iter++)
		{
			delete *iter;
		}

		//set new population
		Population = newPopulation;
		newPopulation = tempList;
	}

	void CalculateFitness()
	{
		sumFitness = 0;

		for (auto iter = Population.begin(); iter != Population.end(); iter++)
		{
			DNA<T> temp = **iter;
			//std::list iterator to int
			int index = std::distance(Population.begin(), iter);
			//calculate fitness of member and add to sumFitness
			sumFitness += (**iter).CalculateFitness(index);

			//update best fitness if current fitness is higher than best
			if ((**iter).fitness > bestFitness)
			{
				bestFitness = (**iter).fitness;
				bestGenes = temp.Genes;
				bestGenesGeneration = generationNumber;
			}
		}

	}

	DNA<T>* ChooseParent()
	{
		float randomNumber = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * sumFitness;

		float randChoice = rand() % 100 + 1;
		//sort population by fitness
		Population.sort([](DNA<T>*  lhs, DNA<T>*  rhs) {return lhs->fitness > rhs->fitness; });
		//50% elitist 50% higher fitness has higher chance to be chosen as parent
		if (randChoice <= 50)
		{

			/////////////////////////////////////////////////////////////////////////////////////////////
			//get random number equal to 25% of size of population
			int randomN = Population.size() * 0.25f;
			if (randomN == 0)
				randomN = 1;
			//choose a random iterator number from 0 to the random number we just got
			int randPlace = rand() % randomN + 0;
			//set iterator to begin of population
			auto iter = Population.begin();
			//iterate to random iterator number place
			std::advance(iter, randPlace);
			//return DNA at iterator position
			return (*iter);
			/////////////////////////////////////////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////////////////////////////////////////

			//random numbers from 0 to 1
			//float w = ((double)rand() / (RAND_MAX));
			//float x = ((double)rand() / (RAND_MAX));
			//int index = std::floor(std::abs(w - x) * (1 + (Population.size() - 1) - 0) + 0);
			//auto iter = Population.begin();
			//std::advance(iter, index); 
			//return *iter;

			/////////////////////////////////////////////////////////////////////////////////////////////

			
			/*  Gives an error if left to run for a while (attempt at roulette selection)
			for (auto iter = Population.begin(); iter != Population.end(); iter++)
			{
				//set temp variable to current iterator
				DNA<T>* temp = *iter;
				//if our random number with fitness is less than our temps DNAs fitness
				if (randomNumber < temp->fitness)
				{
					//return our temp DNA
					return temp;
				}

				//take temp fitness from our random number
				randomNumber -= temp->fitness;
				//}
			}
			*/ 
		}
		else
		{
			//DNA<T>* childDNA = new DNA<T>(dnaSize, getRandomGene, fitnessFunction);
			//childDNA->Genes = bestGenes;
			//return childDNA;
			auto iter = Population.begin();
			return *iter;
		}
	}

private:
	int dnaSize;
	std::function<T()> getRandomGene;
	std::function<float(int)>  fitnessFunction;
};