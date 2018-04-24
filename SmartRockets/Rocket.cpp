#include "Rocket.h"



Rocket::Rocket(DNA<glm::vec2>* dna)
{
	this->dna = dna;
	this->position = glm::vec2(650, 10);
	this->velocity = glm::vec2(0, 0);
	this->acceleration = glm::vec2(0, 0);
	this->completed = false;
	this->crashed = false;
	this->fitness = 0.0f;
	this->index = 0;
	this->reachedEnd = false;
	this->duration = 0.0f;
}


Rocket::~Rocket()
{
}

void Rocket::Update(float deltaTime)
{
	if (index < dna->Genes.size())
	{
		auto iter = dna->Genes.begin();
		std::advance(iter, index);
		glm::vec2 gene = *iter;
		applyForce(gene);
		index++;
	}
	else
	{
		velocity = glm::vec2(0, 0);
		reachedEnd = true;
	}

	if (!completed && !crashed)
	{
		velocity += acceleration;
		position += velocity;
		acceleration *= 0;
		glm::clamp(velocity, glm::vec2(-4, -4), glm::vec2(4, 4));

		duration += 1 * deltaTime;
	}

	positions.push_back(position);
}

void Rocket::Draw()
{
}

void Rocket::applyForce(glm::vec2 force)
{
	this->acceleration += force;
}

